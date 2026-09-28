# 工程_GitHub协作与代码审查

**来源**: 笔录 2026-03-13/2026-03-24《OpenClaw Git 操作》

#Git #GitHub #CodeReview #协作

---

# GitHub 协作与代码审查

## 核心知识点

### 典型 Git 工作流
```
创建分支 → 开发 → commit → push → Pull Request → Code Review → Merge
```

### Git 分支策略
- **main/master**：稳定代码，始终可发布
- **develop**：集成分支
- **feature/xxx**：功能分支
- **hotfix/xxx**：紧急修复分支

### Pull Request 审查要点
1. **功能正确性**：代码逻辑是否符合需求
2. **代码质量**：命名、注释、结构
3. **测试覆盖**：是否有新增测试
4. **性能影响**：是否有性能回退
5. **安全**：是否有安全漏洞

### Git 与 BIOS 项目的特殊性
- BIOS 代码库通常较大（几十 GB）
- 使用 Git LFS（LFS：Large File Storage）处理大文件
- 子模块（submodule）管理 FSP / MdePkg 等公共依赖

### GitHub CLI 操作
```bash
gh pr create --title "feat: add NVMe support" --body "Description"
gh pr review --approve  # 通过 PR
gh pr merge            # 合并
gh issue create        # 创建 Issue
```

### 常见问题
- `refusing to merge unrelated histories` → `git merge --allow-unrelated-histories`
- 合并冲突 → 手动解决冲突后 `git add .` + `git commit`

