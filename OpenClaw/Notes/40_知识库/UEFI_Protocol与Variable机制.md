# UEFI_Protocol与Variable机制

**来源**: 笔录 2026-03-19/2026-03-20《UEFI Protocol / Variable》

#BIOS #UEFI #Protocol #Variable #NVRAM

---

# UEFI Protocol 与 Variable 机制

## 核心知识点

### UEFI Variable（NVRAM）
用于存储 Setup 配置、启动顺序、Boot 变量等。

**常见 Variable：**
| Variable | 用途 |
|---|---|
| `BootOrder` | 启动顺序列表 |
| `Boot0000` ~ | 各启动项配置 |
| `Lang` / `PlatformLang` | 语言设置 |
| `MemoryTypeInformation` | 内存类型配额（首启 Reset 触发点）|

**Variable 操作接口：**
```c
EFI_STATUS EFIAPI EfiGetVariable(
    IN CHAR16 *VariableName,
    IN EFI_GUID *VendorGuid,
    OUT UINT32 *Attributes OPTIONAL,
    IN OUT UINTN *DataSize,
    OUT VOID *Data
);

EFI_STATUS EFIAPI EfiSetVariable(
    IN CHAR16 *VariableName,
    IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes,
    IN UINTN DataSize,
    IN VOID *Data
);
```

### UEFI Protocol
Protocol = 带 GUID 的接口实例，驱动通过它向外提供服务。

**常用 Protocol：**
| Protocol | GUID | 用途 |
|---|---|---|
| `EFI_PCI_IO_PROTOCOL` | gEfiPciIoProtocolGuid | PCI 设备 I/O 访问 |
| `EFI_DRIVER_BINDING_PROTOCOL` | gEfiDriverBindingProtocolGuid | 驱动枚举入口 |
| `EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL` | gEfiPciRootBridgeIoProtocolGuid | PCI 根桥 I/O |
| `gAmiNvmeControllerProtocolGuid` | — | AMI NVMe 控制器句柄 |

**Handle 和 Protocol 的关系：**
```
Handle（设备句柄）
  ├── EFI_PCI_IO_PROTOCOL  ← 安装在此 Handle
  ├── EFI_DEVICE_PATH_PROTOCOL
  └── ...
```

**查找 Protocol 实例：**
```c
LocateHandleBuffer(ByProtocol, &gEfiPciIoProtocolGuid, ...);
// 返回所有安装了 PCI_IO 的 Handle
```

### BDS 中的 PCIe 设备判定逻辑
`IsWarmResetRequiredForPxeBridge()` 在 BDS 中使用：
```c
// 1. 用 LocateHandleBuffer 拿到所有 EFI_PCI_IO_PROTOCOL Handle
// 2. 遍历，检查是否为指定 BDF
// 3. 读 VendorID/DeviceID 确认设备存在性
```
注意：这个函数不是重新做 PCIe 枚举，而是查询已枚举出来的 Handle。

