# OpenClaw_Skill开发与安装

**来源**: 笔录 2026-02-26/2026-03-24《Skill 管理 / Skill Creator》

#OpenClaw #Skill #SkillCreator #clawhub

---

# OpenClaw Skill 开发与安装

## 核心知识点

### Skill 的本质
Skill = 包含 `SKILL.md` 定义文件 + 功能脚本的目录，遵循 OpenClaw AgentSkills 规范。

### 目录结构
```
skill-name/
├── SKILL.md           # 技能定义（名称、触发词、工具说明）
├── references/        # 参考文档
├── scripts/          # 功能脚本
└── ...
```

### 安装方式
```bash
# 从 clawhub 安装
openclaw plugins install <plugin-name>

# 从本地安装（开发中）
openclaw skills install ./skill-name

# 用 clawhub CLI
clawhub install <user/repo>
```

### 关键触发词映射
| 触发词 | 触发的 Skill |
|---|---|
| skill, install, clawhub | Skill 管理 |
| "create a skill" | Skill Creator |
| 飞书, feishu | Feishu 集成 |
| 天气, weather | Weather |
| Obsidian, 同步 | Obsidian Sync |

### SKILL.md 编写要点
- `description`：精确描述何时激活
- `trigger`：列出触发关键词
- `tools`：说明每个工具的用途
- `steps`：复杂技能给出操作步骤模板

