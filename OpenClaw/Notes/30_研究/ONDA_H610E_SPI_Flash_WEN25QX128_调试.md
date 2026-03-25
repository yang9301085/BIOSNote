# ONDA_H610E SPI Flash WEN25QX128 调试

## 基本信息

- **项目**：ONDA_H610E
- **芯片**：WEN25QX128（VEN25QX128），SPI Flash
- **问题**：BIOS 里 Flash 驱动报 Invalid Instruction 错误
- **配合人员**：胡工（芯片厂商）

## 调试过程

### Step 1：移除无效指令 ✅

胡工要求将以下 opcode 从 InvalidInstruction 列表中移除——和规格书核对后，这几个实际上都是支持的指令：

| Opcode | 说明 |
|--------|------|
| 0xC7 | Remove ID / Chip Reset |
| 0x60 | Global Block Lock |
| 0xB9 | Deep Power-down Exit |
| 0x42 | Unique ID Number |

### Step 2：确认 Opcode 套件 ✅

**结论**：代码没有单独新建 opcode 套件，而是使用默认的 `Stm_25PE` 模板。

`SpiIdentify.c:603` 把 Stm_25PE 拷贝到 FlashCommandMenu 定义如下：

| 操作 | Opcode |
|------|--------|
| Write | 0x02 |
| Read | 0x03（FAST_READ_SUPPORT=0 时）|
| Erase 4K | 0x20 |
| Erase 64K | 0xD8 |
| Read Status | 0x05 |
| Read ID | 0x9F |
| Write Status | 0x01 |
| Write Enable | 0x06 |

### Step 3：FAST_READ_SUPPORT 对 Read Opcode 的影响

```c
FAST_READ_SUPPORT = 0  →  Read = 0x03   // 标准 Read
FAST_READ_SUPPORT = 1  →  Read = 0x0B   // Fast Read
FAST_READ_SUPPORT = 2  →  Read = 0x3B   // Fast Read Dual Output
```

### Step 4：最新修改（待上板验证）🔄

胡工要求的最新修改：

| 配置项 | 修改后值 |
|--------|---------|
| `SPI_OPCODE_TYPE_READ_NO_ADDRESS` | 0 |
| `SPI_OPCODE_TYPE_WRITE_WITH_ADDRESS` | 3 |

**待上板测试验证结果。**

## 关键教训

- Intel ME Flex IO 中设置 GPIO 时会覆盖代码中的配置逻辑（此问题在 GPIO_VER3_GPP_E0 分析中已确认）
- ME Flex IO 中的 Flash 配置参数（XML）是最终生效的配置来源

## 相关笔记

- [[2026-03-24]]（日记）
- [[2026-03-25]]（今日日记）
- [[PBX6000_NVMe首启问题调试]]（NVMe 问题，可参考类似调试方法论）
