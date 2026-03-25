
SDRAM、DDR、DDR2、DDR3、DDR4、DDR5手册都会存在这么一张内部结构图，通过图其实就可以知道DRAM容量，工作方式等等。  

如图1是镁光型号为MT48LC16M8A2TG的SDRAM内部结构图，芯片的型号及公司不用太过关注，因为SDRAM和DDR都需要遵守JESD79规则。注意不管是SDRAM还是DDR，同一容量的芯片都会存在4位、8位、16位数据线的芯片，数据线较少的芯片列地址线会多一位。

![图片](https://mmbiz.qpic.cn/mmbiz_png/klO6kQM7mKia7bqxaswlRqiar0pAGdsYLAbvmeM15gVT8bUC5pFQ3WXQian1dgiaXgqda1gLGESHB6s0KQ8eZeJeDw/640?wx_fmt=png&wxfrom=13&tp=wxpic)

图1 SDRAM内部结构图

图2框中部分就是SDRAM的存储结构，每颗SDRAM包含四个bank存储单元，对于该信号的SDRAM来说每个bank包含4096行，每行有512列存储单元，每个存储单元可以存储16位数据。所以内存大小为：

4_4096_512_16bit=22_212_29_16bit=223_16bit=8M_16bit=128Mb=16MB

![图片](https://mmbiz.qpic.cn/mmbiz_png/klO6kQM7mKia7bqxaswlRqiar0pAGdsYLAJOABdJz6DicETzOiaRntAD2lUZtxTYYmuVKj7wRAOvkek5SdMAtqMfMg/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

图2 SDRAM内部结构图

图2的红色方框是SDRAM的存储资源，本质就与excel表格类似，一个表格文件与一颗SDRAM类似，表格内部可以建立多个子表格，而SDRAM内部被划分为4个bank，如图3所示。当bank地址和行地址、列地址都确定时，就可以确定一个存储单元，而SDRAM存储单元的数据位宽与数据总线宽度一致。

![图片](https://mmbiz.qpic.cn/mmbiz_png/klO6kQM7mKia7bqxaswlRqiar0pAGdsYLAJaUcdjq14uLBY6l1qAicLQpzro1zE7KQ5biawqqLCDeMJXKHq7n1y1xA/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

图3 类比SDRAM存储单元

上述了解SDRAM内部存储结构，注意下图4里面关于地址的几部分内容，BA1和BA0为bank的地址线，行地址和列地址共用一组地址线A[11:0]。地址信号进入芯片后经过寄存器寄存，然后将A[11:0]连接到模式寄存器(mode register，因为SDRAM在配置MR寄存器时，配置数据是通过12位地址线传输的)，同时连接到行地址选择器(Row address MUX)，因为自动刷新的行地址是内部的行计数器产生行地址，所以行地址的来源根据指令不同，可能来源于外部的地址引脚，也可能来源于自刷新计数器，通过一个选择器进行选择，数据选择器的输出经过行地址译码模块进行译码，选中对应bank的行地址。如下图橙色部分，两位的bank地址会传输到bank控制逻辑。根据行地址和bank地址，选定接下来需要读取或写入数据的存储单元行、bank地址。

注意：SDRAM可以对不同bank的行进行读写，但在同一bank中，只能激活一行，想要读写另一行时，必须使用预充电指令关闭该bank当前行，然后使用行激活指令打开另一行。

而列地址有地址信号A[11:0]的低9位数据决定，但是注意列地址通过地址寄存器之后，先接入列地址计数器和锁存器，为什么列地址会有计数器？这是因为SDRAM支持突发读写，进行突发读写时，用户只需要输入第一个存储单元的列地址，之后只需要传输数据即可，所以SDRAM在进行突发读写时，内部需要自己计数列地址，输入的列地址相当于列地址计数器的初始值，锁存器就是起保持作用，使列地址在传输数据的一段时间内保持不变。将锁存器输出连接到列地址译码模块进行译码，将外部传输的数据存入SDRAM内部指定bank地址、行地址、列地址的存储单元或者从bank地址、行地址、列地址指定存储单元输出数据。

故在读取或者写入数据时，行列地址需要分别发送，先发送行地址，然后发送列地址，期间bank地址信号保持不变，就能确定存储单元所在的bank、行、列地址。

![图片](https://mmbiz.qpic.cn/mmbiz_png/klO6kQM7mKia7bqxaswlRqiar0pAGdsYLAs6R0N5CxnFg53md7eKj7oKkDmu3YIArqj6ia9odsA7QsobjTOia69wQg/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

图4 地址结构

如图5所示，红框部分位SDRAM的控制逻辑部分，SDRAM只有一个时钟信号clk，时钟使能cke位高电平时clk有效。而片选cs、写使能we、行选ras、列选cas组成指令信号，根据状态不同，组成相关的指令。片选信号位低电平时选中芯片，实际有效的指令就只有8条。

![图片](https://mmbiz.qpic.cn/mmbiz_png/klO6kQM7mKia7bqxaswlRqiar0pAGdsYLA2xR5JvR0J4teYzkZU2A5MYDfgO2WGVhwuFicDS6ft1bSZyfia2eLJNyA/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

图5 控制逻辑

相关指令如下图6所示，当片选为高时，无效。当片选为低电平，其余三个信号为高电平时，此时发送空指令，没有实际作用。在读写数据时，需要先发送行地址和bank地址，然后SDRAM会选中对应bank里对应行，这个操作叫行激活(ACTIVE)，需要片选和行选有效，列选和写使能无效，同时地址线上送出对应的行地址和bank地址。行激活指令发送后，需要等待一段时间tRCD，因为从指令发出到选中对应行是需要时间的。之后发送列地址同时通过写使能确定该指令为读指令或写指令，即片选、列地址、写使能有效时表示向该地址写入对应数据，而写使能无效则表示从该地址读出数据，注意写指令发出的同时需要发送写数据，而读数据会延后读指令CL个时钟周期。当需要读写同一个bank不同行时，需要使用预充电指令(片选、行选、写使能有效，列选无效)关闭当前打开的行，然后在使能行激活指令打开新行。

上述是实现单数据的读写操作，而SDRAM支持突发读写，即对连续地址的数据进行读写时，可以只发第一个数据的地址，后面就可以不用发送地址，只传输数据，直到传输突发长度的数据个数或者遇到突发终止等命令为止。SDRAM支持的突发长度有2、4、8、全页(一行)，而DDR不支持全页突发。

![图片](https://mmbiz.qpic.cn/mmbiz_png/klO6kQM7mKia7bqxaswlRqiar0pAGdsYLArOJ1TOGL48mmRz1tnuaMG1ytQIA7iaKnZSdzsDCkeIgxsBEFwf3lshA/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

图6 相关指令

SDRAM的突发长度，及潜伏期的长度通过模式寄存器(MR)进行设置，上图6中片选、行选、列选、写使能均有效时加载模式寄存器的数值，寄存器数据通过12位地址线进行传输，模式寄存器的含义如图7所示。A2~A0表示读写数据的突发长度，A3表示顺序突发还是交错突发，A6~A4表示读数据时读出的有效数据相对读指令的延迟时钟数，M9表示写操作时单个写还是与使用后面设置的突发长度进行突发写，其余位保留，写0即可。

![图片](https://mmbiz.qpic.cn/mmbiz_png/klO6kQM7mKia7bqxaswlRqiar0pAGdsYLAmjLNxUcenaA1xl0Y8qPxe83Lw63Ja2V40WSGPlQicibRrnOsfCValM3Q/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

图7 模式寄存器

图6中有一个自动刷新的指令(片选、行选、列选有效，写使能无效)，因为SDRAM的存储单元是依靠电容存储电荷来保持数据的，但是电容会进行缓慢的放电，所以每经过一段时间，就需要将存储单元的数据读出，然后重新写入，达到更新的目的，手册给出电容完全放电的时间是64ms，每次刷新指令会对所有bank的同一行进行刷新，而该芯片有4096行，平均15.625us就需要发送一个自动刷新指令。如图7所示，在SDRAM结构中存在一个刷新计数器，该计数器会对刷新的行和bank进行计数，所以用户在发自动刷新指令时，不需要发送自动刷新的行地址。

![图片](https://mmbiz.qpic.cn/mmbiz_png/klO6kQM7mKia7bqxaswlRqiar0pAGdsYLAribicZGF6mPvlUtgbvkc8jNaZiaW1LnwekjgFY8gBbS08JVrkIN76RvQQ/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

图8 自刷新计数器

此处说明一下自刷新、自动刷新、预充电的区别，预充电会把当前打开行的数据进行刷新，然后关闭该行，一般就是用于关闭读写操作后的行。而自动刷新是在芯片正常工作(CKE位高电平)时，为了保证数据不丢失，对芯片的所有行进行周期性的刷新，不像预充电是针对特定行的刷新。有时为了减小功耗，会让SDRAM工作在休眠状态(CKE位低电平)，同时为了防止数据丢失，内部会使用自刷新操作，此时不需要外部提供时钟，SDRAM内部会生成时钟，从而使其执行自己的自刷新周期。

最后就是数据传输相关部分了，如图9所示。读出的数据会从存储单元里读出，因为电容上面的信号比较微弱，会经过放大器放大，然后在输出寄存器上暂存，根据掩膜信号(DQML/DQMH)的状态，确定数据是否输出到数据线上，如果掩膜信号位低电平，则将输出数据输出到数据线上，否则数据线保持高阻态。输入SDRAM的数据首先会寄存在输入寄存器上，然后数据写入IO控制逻辑单元，数据掩膜信号(DQML/DQMH)的状态决定数据会不会被写入到存储单元。

![图片](https://mmbiz.qpic.cn/mmbiz_png/klO6kQM7mKia7bqxaswlRqiar0pAGdsYLA5m4BzmPicWx8gFp2z3rckcBrofYibC3t4Ds3bLLggQZpGib631dB2DBWA/640?wx_fmt=png&tp=wxpic&wxfrom=5&wx_lazy=1&wx_co=1)

图9 输入/输出数据