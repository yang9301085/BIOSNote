在EDK 中的DSC文件中存在inc的引用：
inc是include的缩写，edk用*. inc文件表示这个文件将会被应用到其他文件中
```
!include NetworkPkg/Network.fdf.inc
```
在编译过程中，编译工具会把inc文件中的内容copy出来fang到目标文件中
edk的DSC SPEC对于 `!include` 也有说明，实际上这个文件不一定非要以inc命名，其他的也可以
![[Pasted image 20240108182821.png]]

