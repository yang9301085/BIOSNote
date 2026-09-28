# BIOS_UI_SplitLayout架构

**来源**: 笔录 2026-02-27/2026-02-28/2026-03-02《BIOS Skia 图形开发》

#BIOS #BIOS-UI #SplitLayout #FrontPage #GUI

---

# SplitLayout / FrontPage 架构（AMI TSE/Skia）

## 核心知识点

### SplitLayout 核心概念
SplitLayout = 左右分栏容器：
- 左侧：导航栏（按钮列表）
- 右侧：内容区（按左侧选择切换不同容器内容）

### 内容切换的两种实现
1. **整页切换**：点左侧按钮直接切到另一个完整 Layout → 每次刷新整个页面
2. **子树替换（推荐）**：固定外壳页，左侧栏不变，右侧占位容器里换子树 → 更高效

### BIOS UI 节点树结构
```
RootNode
  └── SplitLayoutNode
        ├── LeftContainer (导航按钮列表)
        │     └── ButtonNode × N
        └── RightContainer (内容占位区)
              └── DynamicContent (点击后替换)
```

### ELINK 在 BIOS UI 中的作用
编译时注入的节点链接表，用于：
- 在 Layout 中注册控件节点
- 关联控件与数据源（HII / Non-Hii）
- 定义回调函数入口

