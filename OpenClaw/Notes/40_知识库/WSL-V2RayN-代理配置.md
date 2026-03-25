# WSL 通过 V2RayN 使用代理

**分类**：工具配置 / 网络
**适用**：WSL2 + Windows V2RayN
**标签**：#WSL #代理 #V2RayN #git

---

## 问题

WSL 中的 git / curl / npm 等命令无法访问 GitHub 等外网，但 Windows 主机代理软件正常运行。

## 根因

WSL2 和 Windows 宿主机处于**不同网络命名空间**，`127.0.0.1` 在 WSL 里指向 WSL 自己，不是 Windows。

## 解决步骤

### 第一步：Windows V2RayN 开启局域网访问

1. 打开 V2RayN → 设置 → 参数设置
2. 找到 **"允许来自局域网的连接"**（Core: 基础设置）
3. 勾选 → 确定 → 重启服务（托盘右键 → 重启服务）

### 第二步：确认局域网代理端口

| 协议 | 本地端口 | 局域网端口 |
|---|---|---|
| HTTP | 10809 | **10811** |
| SOCKS5 | 10808 | **10810** |

（不同版本可能不同，以 V2RayN 状态栏显示为准）

### 第三步：WSL 配置代理

```bash
# HTTP 代理（git 走 HTTP/HTTPS 用这个）
git config --global http.proxy http://192.168.1.19:10811
git config --global https.proxy http://192.168.1.19:10811

# 验证
git clone https://github.com/yang9301085/dive-into-llms.git
```

## 关键地址

| 地址 | 含义 |
|---|---|
| `192.168.1.19` | Windows 宿主机在局域网的 IP |
| `10811` | V2RayN 局域网 HTTP 代理端口 |
| `10810` | V2RayN 局域网 SOCKS5 代理端口 |

## 注意事项

1. **端口不是 7891**：旧版 V2Ray/Clash 常用 7891，新版 V2RayN 用 10808-10811
2. **V2RayN 必须重启**：改完"允许局域网连接"要重启服务才生效
3. **WSL IP 可能变化**：如果 Windows IP 变了要更新
4. **仅限 TCP**：V2RayN 局域网代理不支持 UDP（部分 V2Ray 协议）

## 相关

- [[WSL代理配置问题-2026-03-18]] — 历史记录
- [[WSL代理链冲突排查-2026-03-19]] — 历史记录
