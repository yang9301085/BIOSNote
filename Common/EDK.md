QMEU 绑定虚拟磁盘


![[Pasted image 20240914151709.png]]
- ![[Pasted image 20240914151730.png]]
- ![[Pasted image 20240914151923.png]]
- 保存为img格式
- ![[Pasted image 20240914152003.png]]
- 启动qemu，运行命令
`qemu-system-x86_64.exe -m 1024 -pflash OVMF.fd -drive file=disk-image.img,format=raw`
![[Pasted image 20240914152836.png]]
