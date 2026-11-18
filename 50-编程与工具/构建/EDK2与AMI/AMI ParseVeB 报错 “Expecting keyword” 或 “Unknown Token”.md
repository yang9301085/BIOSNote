
当SDL/INF文件保存为UTF-16的时候AMI ParseVeB会报错
因为：
> 文件用 UTF‑16 编码（每个字符通常占 2 个字节），并且文件开头包含一个字节顺序标记（BOM，Byte Order Mark）。很多老的工具（包括当前 AMI SDL 处理器）只接受纯 ASCII/ANSI 文本；遇到 UTF‑16 BOM 会把前两个字节当作非法字符，于是报 “Expecting keyword” 或 “Unknown Token”。因此需要把此类文件转换成 ASCII/UTF‑8 无 BOM 格式。