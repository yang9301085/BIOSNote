# OpenClaw_Git操作与仓库管理

**来源**: 笔录 2026-03-13/2026-03-24《OpenClaw Git 操作》

#OpenClaw #Git #仓库 #GitHub #部署

---

# OpenClaw Git 操作与仓库管理

## 核心知识点

### Git 基础操作
```bash
git status          # 查看工作区状态
git add .           # 暂存所有改动
git commit -m "msg" # 提交
git push            # 推送到远端
git pull --tags     # 拉取 + tags
git stash           # 暂存当前改动（临时切换分支）
git worktree       # 多工作树（同时在多个分支上工作）
```

### 典型场景

**场景1：暂存当前改动切到其他分支**
```bash
git stash save "WIP: xxx feature"
git checkout other-branch
# ...做其他工作...
git checkout back
git stash pop  # 恢复暂存
```

**场景2：Worktree 方式多分支并行**
```bash
# 新工作树
git worktree add ../feature-branch feature-branch

# 查看所有工作树
git worktree list

# 清理已完成的 worktree
git worktree remove ../feature-branch
```

### Git 与 OpenClaw 集成
OpenClaw 的 workspace 通常也是一个 Git 仓库：
- 笔记目录 `obsidian-notes/` 通常有 Git 管理
- 技能定义也在 Git 仓库中
- 建议：重要改动及时 commit，避免丢失

### GitHub / Gitea 访问问题
- WSL 下无法访问 GitHub → 检查代理设置
- Gitea 自建仓库 → 需配置 SSH Key 或 HTTP Token
- `git clone` 超时 → 网络问题或防火墙

### OpenClaw 项目 Git 规范
- 主仓库：`OpenClawScript/` — 跨平台安装脚本
- 工作区：`obsidian-notes/` — 笔记（通常有独立 Git）
- 建议 commit message 格式：
  ```
  feat: 新功能描述
  fix: 问题修复
  docs: 文档更新
  refactor: 重构
  ```

