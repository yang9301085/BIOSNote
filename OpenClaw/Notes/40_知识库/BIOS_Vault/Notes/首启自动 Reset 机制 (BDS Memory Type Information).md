# 首启自动 Reset 机制 (BDS Memory Type Information)

## 现象

清 CMOS 后第一次启动，机器会自动 reset 一次再正常进入系统。

**这是正常行为，不是 bug。**

---

## 触发条件

三个条件同时满足：

1. `IsFirstBoot == TRUE` (变量里没有 `PreviousMemoryTypeInformation`)
2. `PcdResetOnMemoryTypeInformationChange == TRUE`
3. runtime memory type 配额发生变化

---

## 完整流程

```
开机 N
│
├─ PEI / DXE IPL
│  ├─ 从变量里取 EFI_MEMORY_TYPE_INFORMATION
│  ├─ 按这份配额建立内存类型信息
│  └─ 把结果放进 HOB 传给 DXE/BDS
│
├─ DXE / BDS
│  └─ 注册 "快要启动前" 的回调
│
├─ BeforeBoot 回调触发
│  └─ BdsProceedToBootCallback()
│     └─ SaveMemoryTypeInformation()
│
├─ SaveMemoryTypeInformation() 里做 4 件事
│  │
│  ├─ 1. 探测 PreviousMemoryTypeInformation
│  │    ├─ 找不到 -> IsFirstBoot = TRUE
│  │    └─ 找得到 -> IsFirstBoot = FALSE
│  │
│  ├─ 2. 保存"本次启动看到的旧表"
│  │    └─ 写 PreviousMemoryTypeInformation
│  │
│  ├─ 3. 统计本次真实内存使用量
│  │    └─ UpdateMemoryUsageInformation()
│  │       算出"下次启动应该用的新配额"
│  │
│  └─ 4. 如果新配额和旧配额不同
│       ├─ 写回 EFI_MEMORY_TYPE_INFORMATION
│       └─ 如果：
│          - IsFirstBoot == TRUE
│          - PcdResetOnMemoryTypeInformationChange == TRUE
│          - 改动涉及 runtime memory type
│          则 ResetRequired = TRUE
│
├─ 回到 BdsProceedToBootCallback()
│  ├─ 如果 ResetRequired == TRUE
│  └─ 调 pRS->ResetSystem(EfiResetCold)
│
└─ 开机 N+1
   ├─ PEI / DXE IPL 再次启动
   ├─ 这次一开始就读到"新"的 EFI_MEMORY_TYPE_INFORMATION
   ├─ 按新配额重新建立早期内存布局
   └─ 新配额终于真正生效
```

---

## 代码位置 (AMI BIOS)

| 功能 | 文件 | 行号 |
|---|---|---|
| 首启判断 | `AmiModulePkg/Bds/Bds.c` | 1642 |
| 写 PreviousMemoryTypeInformation | `AmiModulePkg/Bds/Bds.c` | 1659 |
| 重算新配额 | `AmiModulePkg/Bds/Bds.c` | 1698 |
| 置 ResetRequired | `AmiModulePkg/Bds/Bds.c` | 1766 |
| 发起 EfiResetCold | `AmiModulePkg/Bds/Bds.c` | 1822 |

---

## 为什么需要这个机制

**问题**：runtime memory type 的配额需要在 PEI 阶段就确定，但 PEI 阶段不知道实际需要多少。

**解决**：
1. 首启时先用默认配额
2. BDS 阶段统计实际使用量
3. 如果配额需要调整，写变量后 reset
4. 下一轮 PEI 按新配额重来

---

## 核心因果链

```
变量里没历史记录
-> 认为是首启
-> 本轮统计到 runtime memory 配额要调整
-> 当前这轮已经太晚，不能现场改早期内存布局
-> 先写变量
-> cold reset
-> 下一轮从 PEI 开始按新配额重来
```

---

## 相关链接

- [[Cold Reset vs Warm Reset (Intel PCH CF9 寄存器)]]
- [[M.2 PCIe 设备存在检测机制]]

---

*来源: 2026-03-17 NVMe 首启调试会话*
