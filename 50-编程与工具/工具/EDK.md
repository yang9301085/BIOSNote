QMEU 绑定虚拟磁盘


![[EDK-01.png]]
- ![[EDK-02.png]]
- ![[EDK-03.png]]
- 保存为img格式
- ![[EDK-04.png]]
- 启动qemu，运行命令
`qemu-system-x86_64.exe -m 1024 -pflash OVMF.fd -drive file=disk-image.img,format=raw`
![[EDK-05.png]]
