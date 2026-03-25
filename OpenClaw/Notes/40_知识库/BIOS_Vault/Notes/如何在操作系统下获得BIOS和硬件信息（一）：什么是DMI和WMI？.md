我常用hwinfo64来查看电脑硬件状态：

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthNFjqHicFevYiaUEtvloRgrnKIb4IzMag74v6RUOqOSvFJEibXFE9MgQwdA/640?wx_fmt=png&from=appmsg&wxfrom=13&tp=wxpic)

在里面，我们能看到硬件的各种信息：主板厂家、BIOS厂家、内存的品牌和在位信息等等，以及各种Sensor的当前值，如各个CPU内核的温度、风扇的转速、GPU的温度和HDD的SMART内容等等。各位同学有没有好奇，这些软件是怎么做到的？

**一部分信息当然来源于BIOS的输出：SMBIOS和ACPI，还有部分直接读取硬件寄存器（CPU、芯片组、SuperIO、EC），以及借助厂家（如GPU厂家）提供的辅助lib**。本系列就来深入讲讲其中的原理，并借助两套开源代码仓库，结合实际，让你也有能力写出类似的程序，如下面这个：

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthNkmJ8wG43oaPHGx7usDmgGteHW98QRtT7j7VzlMnJv8euuBea4jiaHzQ/640?wx_fmt=png&from=appmsg&wxfrom=13&tp=wxpic)

本系列分两个文章：

- **本文**，主要介绍在Windows和Linux下，读取SMBIOS和ACPI的方法。并结合开源DumpSMBIOS【1】 源码，深入Windows获取SMBIOS信息的两种方法。
    
- **深入篇**，结合开源openhardwaremonitor【2】 源码，深入介绍它获得硬件信息的三种方式，以及为什么这么做，而不是采用其他方法。
    
      
    
    首先，我们从基础概念说起。
    

<h2 style="color:#0066CC">ACPI</h2>

 ![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthN3UFLelxrAo8OkSTrqw3c0Tj8Lz8QYx8gIGhdtMZvq5yxcEZs6Ahqnw/640?wx_fmt=png&from=appmsg&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

我已经不止一次得介绍过ACPI：

[ACPI与UEFI](https://mp.weixin.qq.com/s?__biz=MzI2NDYwMDAxOQ==&mid=2247484333&idx=5&sn=66269e4acfd63d130c8e78d02b60a3d9&scene=21#wechat_redirect)

[为什么ARM Server要用ACPI？ACPI vs DeviceTree](https://mp.weixin.qq.com/s?__biz=MzI2NDYwMDAxOQ==&mid=2247486033&idx=1&sn=057ac7e9a6a2386263c8d117ed0697c5&scene=21#wechat_redirect)

这里提供一个省流版总结：ACPI是Intel、Microsoft、东芝等公司联合开发的一种开放标准，它的核心目标是优化电源管理，同时提供更灵活的设备配置方式。ACPI的引入，使得操作系统可以直接控制硬件的电源状态，而不需要依赖BIOS的预设逻辑，从而提升了计算机的节能能力和管理效率。ACPI主要通过一套标准的数据结构和方法，允许操作系统与固件（BIOS或UEFI）进行交互，其中最关键的部分包括：

**ACPI静态表**：BIOS在系统启动时会将ACPI表加载到内存中，操作系统启动后会读取这些表以了解硬件拓扑和电源管理能力。常见的ACPI表包括：

- DSDT（Differentiated System Description Table）：包含ACPI设备树和方法定义，描述了如何操作和管理硬件。
    
- FADT（Fixed ACPI Description Table）：提供固定的系统信息，例如电源管理的基本能力。
    
- MADT（Multiple APIC Description Table）：描述CPU的中断控制器配置，影响多核处理器的中断管理。
    

**ACPI资源树与动态方法**：采用ASL（ACPI Source Language）语言编写的控制方法，会被编译成为AML（ACPI Machine Language）特殊的字节码。方法如：

- _PTS（Prepare To Sleep）：当操作系统进入睡眠状态前调用。
    
- _WAK（Wake）：在计算机从睡眠模式恢复时调用。
    
- _Qxx（Query）：用于事件查询，如热键触发等。
    

**SMBIOS和DMI**

 ![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthN3UFLelxrAo8OkSTrqw3c0Tj8Lz8QYx8gIGhdtMZvq5yxcEZs6Ahqnw/640?wx_fmt=png&from=appmsg&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

**SMBIOS（System Management BIOS）**规范定义了一系列静态表，它由BIOS生成，包含电脑的各种硬件信息，目的是操作系统就无需测硬件来发现计算机中存在哪些设备。

SMBIOS很容易和DMI搞混，因为SMBIOS最早的名字叫做**DMIBIOS（Desktop Management BIOS）**，因为它是DMI（Desktop Management Interface）的数据基础（什么是DMI后面我们讲）。1996年当时的BIOS厂家带头大哥Phoenix（现在已经落后，它的故事在这里：[风雨40载：BIOS的过去和国产BIOS的诞生](https://mp.weixin.qq.com/s?__biz=MzI2NDYwMDAxOQ==&mid=2247484404&idx=1&sn=4bc441e8419d7e6e871b2a5f055e4435&scene=21#wechat_redirect)）提出DMIBIOS，发布了几版更新后，由老牌非营利组织Distributed Management Task Force(DMTF)接管，改名叫做SMBIOS并维护至今。最新的是SMBIOS 3.8.0 【3】。基本上就是根据硬件发展，做一些兼容性扩充。

DMI，实际上是SMBIOS的一层软件抽象。由操作系统，根据SMBIOS生成的一些列数据结构。它和SMBIOS是分开的，SMBIOS由BIOS生成并更新，启动到操作系统后就不变了。而DMI，是运行时，由OS生成并提供其他组件和上层使用，有自己的标准：DMI 【4】。DMI最新的标准是2.0.1【5】 ，它已经EOL（End of Life）,但还有大量的应用还在用它，如dmidecode。

**ACPI和SMBIOS**

 ![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthN3UFLelxrAo8OkSTrqw3c0Tj8Lz8QYx8gIGhdtMZvq5yxcEZs6Ahqnw/640?wx_fmt=png&from=appmsg&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

有同学可能要问了，为什么BIOS既要给操作系统输出SMBIOS，还要输出ACPI？两个都是硬件抽象，是不是重复了？

是有点信息重复。我的理解是：除了历史原因，两个表的侧重点不同：

- SMBIOS重点在于主板/平台的信息汇报，信息流是单向的，如CPU、内存、PCIe、槽位、主板上料件等等信息，甚至可以和BMC、EC、AMT等结合做资产管理；
    
- ACPI重点在于控制，数据流和控制流是双向的。ACPI的前期重点在于CPU和外设等的电源、功耗和性能汇报和控制，后期加了越来越多的东西，但本质还是控制，报告的信息如设备树，也是为了让OS知道有哪些设备，之间是什么关系，有什么控制方式等。ACPI有很多表，还有很多Method等，Method是控制流自不待言，甚至静态表里面也暴露了很多寄存器或者MMIO，用于操作系统来进行控制。
    

OS实际上是通过ACPI和BIOS来交互的，而不是像SMBIOS那样一过性的单向汇报。但如果想要在OS起来后，再通过动态修改ACPI所在的内存来动态修改ACPI还是不可行的，因为OS会把这块内存的内容copy到自己的一块私有内存中，原来的就不用了。要修改ACPI，我们业内叫做Patch ACPI Table，还是应该在BIOS ReadyToBoot之前，修改对应内存。

**程序代码**

 ![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthN3UFLelxrAo8OkSTrqw3c0Tj8Lz8QYx8gIGhdtMZvq5yxcEZs6Ahqnw/640?wx_fmt=png&from=appmsg&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

Enough原理，是时候说些代码了。我们先看看如何访问SMBIOS数据。

1

SMBIOS

 

在 Linux 中可以通过/sys/firmware/dmi目录或者使用dmidecode 命令，当然命令行最简单：

sudo dmidecode --type bios # 读取BIOS信息 

sudo dmidecode --type 28   # 读取 DMI/SMBIOS 温度传感器信息 

sudo dmidecode --type 27   # 读取 DMI/SMBIOS 风扇信息

因为SMBIOS按照Spec，应该被放置在0xF0000-0xFFFFF之间，我们可以用程序来直接搜索得到：

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthNicia3Y9oocjHoTVv9AhuibUHPlY7j4kIHCM8UTVBDVDFFBboQPeP9qO1A/640?wx_fmt=png&from=appmsg&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

其实就是在指定内存空间寻找“_SM_”这个关键字拉。这些是Linux代码，在Windows下，我们可以看看DumpSMBIOS 是怎么做的。它是个VS Studio代码仓库，比较老，我们可以用VSS打开它或者直接用VS Code打开。关键语句在DumpSMBIOS-Legacy模块里，FindSMBIOS函数中：

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthNZpx6ibalTYMZa7qQRKPeK5qqCXj3N9RR4I9kYDqHOu00jYwltzJJnRA/640?wx_fmt=png&from=appmsg&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

注意，它不仅仅寻找了SMBIOS的关键字_SM_，还寻找了DMIBIOS的关键字_DMI_。

搜索的办法太土了，有没有更方便的方法呢？当然有了，其实Windows已经提供了接口：GetSystemFirmwareTable 。它不仅能寻找SMBIOS，还可以寻找ACPI，还可以寻找ACPI：

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthNia7za5REjaFK9cQ5dFLs7VhCwlibCegntcJFDa8mSt7bDAsfpd4u8k1Q/640?wx_fmt=png&from=appmsg&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

如下面程序，调用 GetSystemFirmwareTable() 读取 RSMB（Raw SMBIOS）后，通过解析 SMBIOS_TABLE_ENTRY_POINT 和 SMBIOS_TABLE_HEADER 结构体，提取 Type 17（Memory Device） 结构中的内存信息，来获得内存在位信息：
```C
#pragma pack(1) // 保证结构体按 1 字节对齐
// SMBIOS 表头结构
struct SMBIOSHeader {
    BYTE Type;
    BYTE Length;
    WORD Handle;
};

// SMBIOS Type 17（内存设备）结构
struct MemoryDevice {
    SMBIOSHeader Header;
    WORD PhysicalMemoryArrayHandle;
    WORD MemoryErrorInformationHandle;
    WORD TotalWidth;
    WORD DataWidth;
    WORD Size;
    BYTE FormFactor;
    BYTE DeviceSet;
    BYTE DeviceLocator;
    BYTE BankLocator;
    BYTE MemoryType;
    WORD Speed;
};

// 获取 SMBIOS 表
std::vectorGetSMBIOSData() {
    DWORD size = GetSystemFirmwareTable('RSMB', 0, NULL, 0);
    if (size == 0) {
        std::cerr << "无法获取 SMBIOS 大小" << std::endl;
        return {};
    }

    std::vectorbuffer(size);
    if (GetSystemFirmwareTable('RSMB', 0, buffer.data(), size) == 0) {
        std::cerr << "无法获取 SMBIOS 数据" << std::endl;
        return {};
    }
    return buffer;
}

// 解析 SMBIOS 表，获取内存设备信息
void ParseSMBIOS(const std::vector& smbiosData) {
    if (smbiosData.empty()) {
        std::cerr << "SMBIOS 数据为空" << std::endl;
        return;
    }
  
    BYTE* data = (BYTE*)smbiosData.data();
    DWORD length = smbiosData.size();
    DWORD offset = 8; // 跳过 SMBIOS 入口点结构

    int memoryCount = 0;
    while (offset < length) {
        SMBIOSHeader* header = (SMBIOSHeader*)(data + offset);
        if (header->Type == 127) break; // Type 127 标记 SMBIOS 结束
        if (header->Type == 17) { // Type 17 = Memory Device
            MemoryDevice* mem = (MemoryDevice*)header;
            if (mem->Size != 0xFFFF) { // 0xFFFF 表示未安装
                memoryCount++;
                std::cout << "内存条 #" << memoryCount << ":" << std::endl;
                std::cout << "  插槽: " << (int)mem->DeviceLocator << std::endl;
                std::cout << "  物理 Bank: " << (int)mem->BankLocator << std::endl;
                std::cout << "  大小: " << ((mem->Size & 0x8000) ? (mem->Size & 0x7FFF) * 1024 : mem->Size) << " MB" << std::endl;
                std::cout << "  速度: " << mem->Speed << " MHz" << std::endl;
            }
        }

        // 跳过当前表 + 结构体字符串区域
        BYTE* p = data + offset + header->Length;
        while (*p != 0 || *(p + 1) != 0) p++; // 跳过字符串部分
        offset = (p + 2) - data;
    }

    std::cout << "总共检测到 " << memoryCount << " 根内存条" << std::endl;
}

int main() {
    std::vectorsmbiosData = GetSMBIOSData();
    ParseSMBIOS(smbiosData);
    return 0;
}
```

输出为：
```Text
内存条 #1:
  插槽: 0
  物理 Bank: 0
  大小: 8192 MB
  速度: 3200 MHz
  
内存条 #2:
  插槽: 2
  物理 Bank: 1
  大小: 8192 MB
  速度: 3200 MHz
  
总共检测到 2 根内存条
```
DumpSMBIOS 是怎么做的呢？它在DumpSMBIOS模块的DumpSMBIOS.cpp里：
![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthNHTPuzrakbEagQvxzqpEpJjTgsOXa1AchMat8iaSEzHFjGDoaQ5VqtMw/640?wx_fmt=png&from=appmsg&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

输出结果如下：

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaL9GA3fBH5G9TPk2ibIdCthN2Ld4o75V7zsjs3qKymk8tSKrQM4CBhvYpGtcZUGq3dYnejMGpxw8Gg/640?wx_fmt=png&from=appmsg&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

_节选，不全_

ACPI

在 Linux 下可以读取 /sys/firmware/acpi/tables/ 目录获取 ACPI表，非常简单，这里就跳过了。在Windows下，推荐WMI（Windows Management Instrumentation）来访问ACPI。WMI (Windows Management Instrumentation) 是 Windows 系统中一个重要的管理基础结构，用于访问系统管理信息。在硬件监控中，它是一个非常有用的工具。

WMI的接口是COM接口，没用过的同学可能不太熟悉。COM接口实际上是Windows的底层结构，很多功能实际上都是基于COM接口，我们以后再讲。先说今天的主题，如我们在ACPI中定义了一个温区，用_TMP返回一个固定的温度（Demo）：

```C
Device (THRM)
{
    Name (_HID, "ACPI0007")  // ACPI 设备 ID
    Name (_TMP, Method (0, NotSerialized)
    {
        Return (0x2D) // 返回假定的温度值 45°C
    })
}
//我们可以用WMI来访问它：
#pragma comment(lib, "wbemuuid.lib")
double GetCPUTemperature() {
    HRESULT hres;
    // 初始化 COM
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cerr << "COM 初始化失败: " << std::hex << hres << std::endl;
        return -1;
    }
  
    // 初始化安全性
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    if (FAILED(hres)) {
        std::cerr << "COM 安全初始化失败: " << std::hex << hres << std::endl;
        CoUninitialize();
        return -1;
    }

    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);
    if (FAILED(hres)) {
        std::cerr << "创建 WMI 连接失败: " << std::hex << hres << std::endl;
        CoUninitialize();
        return -1;
    }

    IWbemServices *pSvc = NULL;
    hres = pLoc->ConnectServer(BSTR(L"ROOT\\WMI"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
    if (FAILED(hres)) {
        std::cerr << "连接 WMI 失败: " << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return -1;
    }
  
    hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(hres)) {
        std::cerr << "设置 WMI 权限失败: " << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return -1;
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(BSTR(L"WQL"), BSTR(L"SELECT * FROM MSAcpi_ThermalZoneTemperature"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
    if (FAILED(hres)) {
        std::cerr << "查询温度失败: " << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return -1;
    }

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;
    double temperature = -1;
  
    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0) break;
        VARIANT vtProp;
        hr = pclsObj->Get(L"CurrentTemperature", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            temperature = (vtProp.uintVal - 2732) / 10.0; // 转换为摄氏度
            VariantClear(&vtProp);
        }
        pclsObj->Release();
    }
  
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
    
    return temperature;
}

int main() {
    double temp = GetCPUTemperature();
    if (temp >= 0) {
        std::cout << "CPU 温度: " << temp << "°C" << std::endl;
    } else {
        std::cout << "无法获取 CPU 温度" << std::endl;
    }

    return 0;
}
```

常用 WMI 命名空间有：
- root\CIMV2 ：最常用，包含大多数系统信息
- root\WMI ：包含温度、电源等传感器数据
- root\Hardware ：硬件相关信息

其实程序非常简单，先用COM接口发现Locator，用Locator连接WMI service，然后就可以以类似SQL语言来查询感兴趣的东西了。
除了温度，我们也可以来查询风扇转速
![[c434f585d88b6d0c0d1c17e364607118.png]]
注意这次是Locate “root\CIMV2”下的服务。用“root\CIMV2”还可以访问SMBIOS提供的内存在位信息，非常强大：
![[574d58c87c6e962aa6d3597039e8450d.png]]
输出结果为：

![[069b8c8f312085b3c5630b329c37f73c.png]]
还有没有别的方法？当然还有，还可以使用我们下文会讲到的OpenHardwareMonitor，它由一个Lib和一个界面组成。我们可以直接用它里面的函数来完成很多操作。
![[06ff89a60dc4c90bf60a1ad6b5e365de.png]]
