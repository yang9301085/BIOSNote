
> [!note] 历史学习记录
> 本文来自 2026 年 3 月的笔记与会话摘录，2026-11-18 提取归档。技术结论、寄存器定义和项目状态按原记录保留，使用前需结合对应平台源码、手册及实测核对。

# Git 基础操作与多工作树

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

### GitHub / Gitea 访问问题
- WSL 下无法访问 GitHub → 检查代理设置
- Gitea 自建仓库 → 需配置 SSH Key 或 HTTP Token
- `git clone` 超时 → 网络问题或防火墙
