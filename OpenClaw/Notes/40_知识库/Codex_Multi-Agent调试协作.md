# Codex Multi-Agent 调试协作模式

**来源**: 笔录 2026-03-24/25《BIOS-调试打印》系列

#Codex #调试 #Multi-Agent #BIOS

---

## 核心知识点

### Multi-Agent 调试是什么

用多个 Codex 子 Agent 并行工作，每个子 Agent 负责一个"Lane"（车道），分工协作完成复杂调试任务。

**典型分工示例（PBX6000 NVMe 首启调试）**：

| Lane | 职责 | 关注点 |
|---|---|---|
| Lane 0 | 统筹协调 | 汇总各 Lane 结论，主线分析 |
| Lane 1 | 编译依赖 & 接线复核 | 检查 patch 需要改哪些 INF/library |
| Lane 2 | 日志点位 & 噪声控制 | 验证最小打点 patch 是否足够小 |
| Lane 3 | 假说验证 | 验证"LTSSM 训练未完成"假说 |

### Multi-Agent 实现方式

**方式 1：并发 Sub-Agent（OpenClaw 模式）**
```
Main Agent（主会话）
  → Spawn Sub-Agent 1 (Lane 1) 并行
  → Spawn Sub-Agent 2 (Lane 2) 并行
  → 各 Agent 完成后汇总
```

**方式 2：Codex Session 分工（手动模式）**
- 每个 Lane 开启独立的 Codex 终端 Session
- 通过共享文档（如 Obsidian）交换结论
- 主会话（启仁）负责汇总

### Lane 职责定义模板

```
Lane X [职责名]
固定分工：[具体负责什么]
只读关注：[验证目标]
工作方法：[只读 / 编译 / 分析]
输出：结论（写到共享文档）
```

### 关键优势

1. **并行加速**：4 Lane 并行 ≈ 4倍速
2. **专注分工**：每个 Lane 只管一件事，不容易跑偏
3. **假说验证**：多 Lane 可同时验证不同假说

### 常见坑

- **Lane 之间缺乏同步**：各 Lane 输出格式不一致，汇总困难
- **职责重叠**：Lane 边界不清晰，重复劳动
- **主会话成为瓶颈**：汇总不及时，Lane 空闲等待
