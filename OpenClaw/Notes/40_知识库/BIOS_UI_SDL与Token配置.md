# BIOS_UI_SDL与Token配置

**来源**: 笔录 2026-02-28/2026-03-03/2026-03-13《SDL / Token 配置》

#BIOS #BIOS-UI #SDL #Token #HII

---

# BIOS UI：SDL 与 Token 配置

## 核心知识点

### SDL 是什么
SDL = System Description Language，AMI 用于描述 BIOS Setup 选项的语言。
- 类似 JSON/YAML 的键值结构
- 定义 Setup 页面的控件、默认值、条件跳转

### 典型 SDL 结构
```sdl
TOKEN = "CUSTOMER_SUPPORT" "=" "1" {
  HELP = "Enable customer support mode";
  DEFAULT = 0;
};

TOKEN = "BIOS_VERSION" "=" "BIOSUIT_" {
  VARSTORE = VarStoreGuid;
  OFFSET = 0x10;
  SIZE = 0x04;
};
```

### Token 覆盖机制
Token 值决定各功能模块的行为分支：
- `CUSTOMER_SUPPORT = "0"`：走默认分支逻辑
- `CUSTOMER_SUPPORT = "1"`：走定制分支

修改 Token 的方法：
1. 直接改 `.sdl` 文件 → 重新编译
2. 在 `.inf` / `.dec` 中定义 PCD → 代码中动态读取

### ELINK 与 SDL 的配合
```
.sdl 文件  →  定义 Token 和控件结构
     ↓
.c 文件中的 ELINK 宏 → 把 Token 和控件节点注入
     ↓
编译时生成 gTokenToGuid[] 表
     ↓
运行时通过 TokenID 查 HII Browser 控件
```

### HII（Human Interface Infrastructure）
HII = BIOS Setup 页面的框架：
- `HiiDatabase` — 存储所有 Setup 字符串、控件定义
- `HiiForm` — 单个 Setup 页面
- `HiiPackage` — 打包的 HII 数据（String / Form / Variable）

### BIOS Version Token 定制
改 `BIOS_VERSION` Token 可在 UI 上显示定制信息：
```sdl
TOKEN = "BIOS_VERSION" "=" "PBX6000_v1.0" {
  HELP = "Custom BIOS version string";
};
```

