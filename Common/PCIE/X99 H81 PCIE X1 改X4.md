BIOS 中PCIE 配置
origin：
![[Pasted image 20240124181202.png]]
原PCIE Port configuration 为 4x1

新电路图：
![[f412c394cb95c574152261d7b2406d2.png]]

PCIE 中4x1表示PCIE Port4个port 独立出来，每个port连不同的地方
PCIE 1x4 表示PCIE port 4个port为一组，这一组只连一个东西


在设置成X4的时候没有生效，查看设备管理器中USB下面有黄标。
切换其他的PCIE配置如：1x2后功能ok，但软件中还是显示x1，切换成2x2后也有黄标
查看配置详细说明发现：1x2和4x1的port 4都是Enable
				     2x2和1x4的port 4都是Disable
问EE说PCIE并没有做 PCEI lane reversal，但把PCIE port 1 lane reversal 打开配合PCIE 1x4功能正常
最后把PCIE port 1 lane reversal disable + USB3 Port 2/3 PCIE port 1/2 mode（USB复用功能关闭）
+PCIE 1x4 功能正常，测试软件也显示PCIE x4

summary：
