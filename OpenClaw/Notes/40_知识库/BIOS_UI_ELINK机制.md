# BIOS_UI_ELINK机制

**来源**: 笔录 2026-03-03《AMI TSE 布局排障》

#BIOS #BIOS-UI #ELINK #GUI #HII

---

# ELINK 机制详解

## 核心知识点

### ELINK 是什么
ELINK = Entry LINK，AMI 的编译时节点注入宏机制。
用于在编译阶段把分散在各处的 UI 节点（控件、布局、回调）链接成完整树。

### 工作原理
```c
// 在 .sdl 中
ELINK(GiveFrontPageLayout)
  REGISTER_NODE(ButtonNode, DisplayID_XXX, Callback_XXX);
  REGISTER_NODE(StaticTextNode, DisplayID_YYY, Callback_YYY);
END_ELINK
```
编译时，ELINK 工具把所有 `Give*Layout` 引用收集，生成统一节点表供运行时使用。

### ELINK 排查编译问题的顺序
1. 追 `*_NODE_LIST` ELINK 宏链（节点是否全部注册）
2. 查 `.cif` / `.sdl` / `.veb` 模块注册链（注册是否成功）
3. 核对 `Build` 下生成物是否正确导出和链接

### 常见错误
- `error C2065: 'SPLIT_LAYOUT_NODE' : undeclared identifier` → SDL 中节点未在 .h 中声明
- `ESA_CONTROL_NODE gSplitLayoutNodes[] 重复定义` → 多处 ELINK 注入了同名节点表

