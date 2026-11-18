最近遇到了一个关于CPU 功耗的问题：
Intel 12400F CPU的鲁大师跑分低于正常值
正常跑分在55W左右，但我的BIOS跑分只能跑到49W

因为我用我的这版BIOS试过很多个CPU
也在其他的板子上试过CPU的跑分
结果都是这个现象，所以就不谈平台对它的影响了。

下面是对Intel平台来说，影响CPU power consumption的参数：

• IMON Slope
• 定义：IMON Slope是BIOS中的一个参数，用于调整CPU功耗的显示值。
• 影响：通过调整IMON Slope，可以改变CPU报告给系统监控软件的功耗值，从而影响功耗墙的触发。

• IMON Offset
• 定义：IMON Offset是BIOS中的一个参数，用于微调CPU功耗的显示值。
• 影响：IMON Offset可以增加或减少显示的功耗值，影响功耗墙的触发。

• AC Loadline(ACLL)
• 定义：AC Loadline是一个虚拟电阻值，用于模拟CPU在不同负载下的电压变化。
• 影响：ACLL影响CPU在不同负载下的电压请求，从而影响功耗。

• DC Loadline(DCL)
• 定义：DC Loadline用于模拟降压，得到一个更接近于真实电压的值，用于计算功率。
• 影响：DCL影响系统监控软件显示的电压值，使其更接近CPU的实际工作电压。

~~• PL1(Long Duration Power Limit)~~
~~• 定义：长期功耗限制，定义了CPU在长时间内可以维持的最大功耗。~~
~~• 影响：降低PL1值可以减少CPU的平均功耗，但可能会影响性能。~~

~~• PL2(Short Duration Power Limit)~~
~~• 定义：短期功耗限制，定义了CPU在短时间内可以超过PL1的最大功耗。~~
~~• 影响：调整PL2值影响CPU在高负载下的峰值功耗。~~

~~• TDP(Thermal Design Power)~~
~~• 定义：热设计功耗，是CPU在最大负载下预期产生的热量。~~
~~• 影响：TDP值越高，CPU在高负载下可能消耗的功率越大。~~

~~• CPU Core Voltage~~
~~• 定义：CPU核心的工作电压。~~
~~• 影响：降低核心电压可以减少功耗，但也可能影响性能和稳定性。~~

~~• CPU Multiplier~~
~~• 定义：CPU的倍频，影响CPU的运行频率。~~
~~• 影响：增加倍频可以提高性能，但也会增加功耗。~~

划掉这些外界因素，PL1，PL2的值远大于12400F的功耗，也不考虑。
最后只剩下AC/DC LoadLine，IMON Slope、Offset了

计算功耗的公式：
$P_{\text{static}} = I_{\text{leakage}} \times V$
$P_{static} -> 静态功耗$
$P_{lakeage} -> 漏电流$
