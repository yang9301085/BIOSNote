问题：Veb编译的时候ELINK太多导致windwos命令长度太长导致整条命令被截断

真正原因是**生成脚本把整个 IMAGE_MAP_ELINK1 当成一整行命令传给 echo**（见 SupportFiles.mak）。  
在 Windows 下命令行有长度限制（约 8KB），加上这两个条目后整条 echo -e "…$(IMAGE_MAP_ELINK1)…" 超长被截断，导致 GUIDMappings.c **只生成了前半段**、末尾右大括号没写进去，于是编译报 C1075。你注释掉这两条后，长度降回阈值内，生成就完整了。

# Windows 下命令行有长度限制（约 8KB）
