汇编、链接、运行程序
```plantuml
start
:源文件;
note left: asm files
->汇编器;
fork
	:目标文件;
	note left: 汇编器读取源文件生成目标文件
fork again
	:列表文件;
	note right:汇编器也会生成列表文件
end fork
split
	->链接器;
	:可执行文件;
split again
	-[hidden]->
	:链接库;
end split

->OS加载器;
:输出;
end
```
