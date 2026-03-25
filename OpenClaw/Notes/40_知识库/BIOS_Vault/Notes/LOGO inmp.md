# UEFI Logo显示原理、源码解读和定制实践 已付费

原创 高书记 老狼 UEFI社区

 _2024年10月22日 12:04_ _上海_

前言

通过本文，您将收获如何将原生朴素的EDK2 Logo界面，定制成商业化产品的界面。并跟读代码和了解原理。推荐下载文章最后的代码包，获得最佳体验。

  

  

编者按

  

长久以来，本专栏偏重于UEFI理论，而在某种程度上缺乏实践指导。而开源Edk2有大量的源码，但是缺少源码的解释说明，在遇到实际问题上，大家往往无处下手。恰恰这些，才往往是BIOSer日常工作中需要真正面对的问题和痛点。

**针对这个情况，我们收集了BIOS产品中常见的需求，准备结合BIOSer真实的需求，邀请UEFI领域内的资深专家们，有些还是edk2的某些功能的原作者和现在维护者，来深入讲解如何高效的改造Edk2代码以满足产品的要求****。**

本系列采取连载的方式，一篇文章聚焦一个生产环节或者切片，并和源码讲解配合，希望能够讲深讲透。

希望本系列可以对读者工作有所助益，为了激励各位专家继续创作，本系列采取收费的阅读的模式，花费不多，希望大家觉得物有所值，也拜托读者尊重知识产权。对内容的建议，或者还希望了解哪些方面，欢迎后台留言，帮助我们更好的完成后续的文章，感谢大家！  

本文是讲解Logo显示相关模块的源码和应用，这些模块不止局限于图片，也会涉及到字体和进度条的图形化显示。希望通过本文的说明，能够给大家提供一些思路，在今后的工作中，再次遇到类似的需求时，可以做到有的放矢。

图一是Edk2 Emulator仿真平台的启动界面，中间显示的就是Logo，左上角是提示信息，最下面是进度条，非常的简单直观；图二是换成全屏的图片，可能你只是换了图片，怎么效果就显得如此突兀，考虑使用透明字体的方式显示；图三是以透明的方式显示字体，这样字体可以浮在图片上，效果就比较自然；图四是进度条改成完全填充的，并且同时显示进度的百分比。通过这些定制，最终的启动显示基本可以满足多数的产品BIOS需求了。  

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7pAvJBsoDiaSYBvWib7HvUAzJRIcZaYUgY4x1FFVlqoEnTjCO1KgJ6p5w/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

图一：EDK2 Emulator原生的启动界面

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7If6yPicMq3LJSGEyuBq4pBiacor0c70NFvDDOWc7hhKkv5LwU0o0ySZQ/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

图二：全屏Logo

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ728vjFNpTrC6o5crg0wSgiaRH2ufduDTXrafOjYffTxaVqDPXkx7ibZBA/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

图三：透明字体

  

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ75BoUoia9TrvzsOJ5CkbUyx6icAh8A3lzqLhYeC55RKlVbmMDib1Z6EA7Q/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

图四：填充进度条和百分比显示

本文通过分析显示模块的源码，介绍这些常用的显示定制方法（包括颜色、位置、透明字体等），从而达到最终的显示效果。在文章末尾我们还提供了完整的代码改动，供大家参考使用。Edk2中Logo相关模块如下表所示，基本都是通用模块，平台库只需要调用BootLogoLib的库函数就可以显示Logo和进度条。  

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJPwdiaGlX11NmMKIqSgQ9acpAD1v1xN6IRKQtRfMINIJia4pteRsC4Nqyib1iabYNINrfFV56UzaV2Hg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

本文重点对BootLogoLib和Logo两个模块的源码逐行分析，在分析的过程中，会引出具体的使用场景，并介绍适用的定制方法。文中的源码来自开源Edk2的主干 https://github.com/tianocore/edk2，大家可以下载源码对比来看，以便于更好的理解。

  

**Logo模块**

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7s2ZkpwWibKNLeTaORCJX1AlhQYcFWvcnQQ2mTEsXNLzFxapH4mYvSVQ/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

1

源码分析

接下来我们进入正题，首先解析Logo模块，这个也是在产品中定制最多的模块。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7jcRria02wBn09w5rKvWFT2urTZUw6Lkwgw1c2GzsGTyonshIQAic4iaxA/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

它的INF中 [Sources] section中有一个Logo.idf文件，这个文件是用于描述图片文件的，Edk2 Build系统会把这个文件中的图片封装成UEFI HII Image数据包，就如同把xxx.uni文件中的字符串封装成HII String数据包一样的。[Defines] section中的UEFI_HII_RESOURCE_SECTION设置为TRUE，那么生成的二进制HII 数据包会直接填充到最终EFI文件中。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7Tt1Z10Qt7nz4via9iczC3eLKiaTP88qoa7ialfZxKHrf12TyOibicJ72EXfA/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

Logo.idf文件定义很简单，只需列出来图片的文件名，并给它定义对应的宏（如IMG_LOGO），这个宏在C代码中就指代这个图片。如果需要多个图片的话，把它们都列在这里，然后还要把对应的文件名添加到INF文件中的[Sources] section里。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7EX8uQgaBmYO8XDn184ucQyXjUyLeGT7bMWt8nhuavfsxVKb9gwiaWyw/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

主函数都在Logo.c中，这里依赖的HiiImageEx protocol，用于从HII数据包中获取和更新图片，这个Protocol的实现是在HiiDataBase模块中，具体的实现细节就不在这里展开了。全局变量mLogos是一个可变数组，列出来需要显示的Logo图片。图片的ImageId IMG_LOGO指代在idf文件中对应的图片Logo.bmp；图片的Attribute EdkiiPlatformLogoDisplayAttributeCenter是图片显示的位置属性（居中显示），支持的位置属性都定义在MdeModulePkg\Include\Protocol\PlatformLogo.h（如下所示）；图片的OffsetX和OffsetY是在屏幕上显示的起始位置偏移，通常都是0，整个图片完整显示。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ79fCwxAAiasIAu3ZuRzwRGBbq6GeEDRTIWaF6YVT1VHFa3dr9Gk23b9w/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

  

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7Jf2UGqe0X6EW4eYbNwUbxBfnjeLuKQvVJLXrI71eX0CLPibiacsT3yuw/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

Logo模块对外安装了PlatformLogo Protocol，这个Protocol只有一个函数GetImage()。GetImage()的实现是遍历mLogos数组，调用HiiImageEx protocol的GetImageEx()，基于ImageId获取每一张图片的显示数据。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7InOE2zQgsA9COWaViaqbRNcIYyiaOsPoiaMy8Dkj8Ga5dPtARLAgPUmgw/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

InitializeLogo ()是模块的入口函数，首先是Locate HiiDataBase Protocol，用于安装它的HII数据包。这个模块的HII数据包是存放在EFI image中，EFI image在被DxeCore加载的时候，它的HII数据包会被安装在ImageHandle的HiiPackageList Protocol上，所以这里可以通过HiiPackageList Protocol获取到它对应的HII数据包 PackageList。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7pnQ2suP63K5PkZkMb8qia29PbnlTbtXTsOEntwoRuasOeNC1bfUX5wQ/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

成功获取HII数据包以后，通过HiiDataBase NewPackageList()把HII数据包安装到HII数据库中，并获得mHiiHandle，之后通过mHiiHandle获取对应的图片的内容。如果HII数据包安装成功的话，最终会安装PlatformLogo Protocol。这个Protocol会被BootLogoLib使用，用于获取图片的信息，并把图片显示在屏幕上。

至此，整个模块分析完了。从代码来看，逻辑并不复杂，总结起来就是两点。首先图片是封装成HII数据包，通过HIIImageEx Protocol获取图片，图片转换成BLT数据的逻辑都是在HiiDataBase模块中实现的，这里只需要调用公共的接口；另外就是Logo模块把图片的HII数据包安装到HII数据库后，安装PlatformLogo Protocol提供GetImage()获取图片的显示信息。  

2

需求定制

这个模块对外提供GetImage()函数，所有图片信息的定制都是在这个函数里完成，下面介绍一下常见的产品需求和对应的定制方法，文章末尾也提供了示例源码，方便大家理解。

- **图片顶格显示**
    

mLogos变量设置了图片的显示属性，目前是居中显示EdkiiPlatformLogoDisplayAttributeCenter，可以修改为EdkiiPlatformLogoDisplayAttributeLeftTop，让图片从屏幕的左上角开始显示。

- **多张图片显示**
    

在Logo.idf, Logo.inf 中添加更多的图片，并在mLogos数组中把新的图片列出来，根据需求分别设置不同的显示属性，避免图片重叠。

- **平台定制显示**
    

产品BIOS往往是一个ROM文件，运行在不同型号的客户平台，需求是在不同的客户平台上显示不同的Logo。需要在Logo模块里添加所有用到的图片，和多张图片显示的添加方式是一样的，只是在mLogos数组中描述图片的时候，这些图片的显示属性是可以相同的，因为它们是独立显示的，不会重叠。图片添加完成以后，还需要额外修改GetImage()中的逻辑，基于当前BoardId，在mLogos数组中选择对应图片的ImageId，这样返回的就是匹配当前平台的图片。

- **多种图片格式**
    

图片转换成UEFI的BLT数据结构是在HiiDataBase模块中实现的，默认支持的图片格式只有BMP。根据UEFI规范，HII Image的数据包还支持JPG和PNG的图片格式。图片格式的解析代码需要封装成HII Image Decoder Protocol，然后被HiiDataBase模块调用。Edk2的源码中并没有提供JPG或PNG图片的Decoder Protocol。如果需要支持JPG或者PNG图片的话，一种可行的方法是在操作系统下把图片转成BMP格式，然后再放到Logo模块里。这个方法带来的问题是转换后的BMP文件Size往往比较大，比较占用BIOS ROM的空间。直接的方法是添加JPG和PNG图片的解析模块，JPG和PNG是常见的图片格式，github上都有开源的解析代码，可以移植到Edk2项目中使用的。本文的末尾给出了JPG和PNG在github上开源项目的链接，我们已经把它们移植到了Edk2项目，并做了基本的功能验证。

如果项目里已经有JPG和PNG图片的Decoder Protocol，那么只需要把用到的图片添加到Logo模块里。只是有一点需要注意的是，图片的后缀只能是.bmp, .jpg, .png，分别对应不同格式的图片。

- **图片的替换**
    

Logo模块包含的是默认Logo图片，产品项目中的需求是替换Logo图片。一种需求是直接编辑BIOS ROM文件，更新Logo图片；另一种需求是在BIOS运行的机器上，通过工具更新Logo图片。解决方案的大致思路是在BIOS ROM文件里预留一块空间（比如64KB），用来存放用户设定的Logo图片。Logo模块的入口函数会检查这块预留空间，如果预留空间里有图片的话，需要先把图片转成BLT数据结构，然后调用HII ImageEx Protocol的SetImageEx()函数更新ImageId对应的图片，这样之后使用GetImageEx()就可以获取更新后的图片数据了。

**BootLogoLib模块**

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7s2ZkpwWibKNLeTaORCJX1AlhQYcFWvcnQQ2mTEsXNLzFxapH4mYvSVQ/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

1

源码分析

BootLogoLib库是一个平台无关的通用库，提供了显示Logo和进度条的库函数。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ74iaibb0PGzsNSRrN5zJNAT27Mc5ibXKDOSu211quHgvayajLDcyF4Otmg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

BootLogoEnableLogo () 是显示Logo的库函数，它没有参数，只能在BDS阶段显示驱动安装以后才可以被调用。通常是在平台的PlatformBootManagerLib库的

PlatformBootManagerAfterConsole()函数中被调用的。下一个章节的PlatformBmLib模块中会介绍它何时被调用。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7WlmnZg626uZTSFuuYBhPUMgRiaStug321HjRf169ENVCZJS1e4sWIYw/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

首先是找到PlatformLogo Protocol，它是由Logo模块安装的，用于获取一个或多个Logo图片；然后是找到GOP Protocol，获取显示屏幕的当前分辨率；最后是查找BootLogo 或者 BootLogo2 Protocol，这个Protocol是由BootGraphicsResourceTableDxe模块安装的，用于保存显示的Logo图片信息，最后在系统启动时上报ACPI BGRT表。BGRT表的作用会在下一个章节中介绍。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7wMfesSOUaw6EP4vXmq5q2ica0xBwNYe8UfnJiawbY7cQELTXOmBBUhdA/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

想比较BootLogo Protocol，BootLogo2 Protocol 多了一个GetBootLogo() API，这个函数可以获取当前已经显示的Logo图片信息，目前的用法只有在Capsule更新的时候，显示更新的进度条需要显示在Logo的下面，所以通过这个API获取显示Logo的位置信息。接下来的逻辑是在屏幕上把光标关闭，在Logo显示的时候不需要显示光标。最后就是通过GOP获取当前屏幕的显示分辨率，基于当前的分辨率，以便于确定Logo图片的显示位置。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ76t07JnMy1PLFqQgMY1K1wvCyia7TArYia2CQicIpvMypGicodwRIER1Qug/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

这里是一个while死循环，通过GetImage()函数依次获取Logo图片，直到返回失败。Instance是从0开始的，也就是从第一个Logo图片开始找。GetImage()函数返回了图片的数据、显示属性（位置）以及显示的偏移坐标。如果OffsetX 或者 OffsetY 不是0的话，说明图片只需要部分显示出来。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7ATMvHYL97JRSA0oc8J4FAOqBsGul8iaQx9LOwat2c5ic2CTZhd4FfuGQ/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

基于图片的位置属性，计算图片在屏幕上显示的起点位置。LeftTop是左上角，也就是从屏幕的起点进行显示。CenterTop是上居中，RightTop是右上角对齐，CenterLeft是左居中，Center是屏幕居中，也是最经常使用的，等等各种位置信息。图一的显示就是屏幕居中。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7CzHy0V3DJ9OlbEPjCDwL8m2PWovXFicwXmwMwvQBcBOeglQMjOIF7tg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

获取图片数据和位置信息以后，调用GOP的Blt()函数，使用BltBufferToVideo操作，把Blt数据显示在屏幕上。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7ibB5YcZMU9QacMxrMicrI2uZ6FC0YK7rMyIibFNbkoEJhba5BDDK0zDMA/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

显示成功以后，需要记录已经显示的Logo所占用的屏幕区间，如果只有一个Logo的话，只要记录这个Logo所在的区间；如果有多个Logo的话，需要把Logo所占用的空间合并。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7ic4YAAYzQDDWQXDJRjLeSeciawbGtAica168sq3BLG3hytRBSicGd6xaAA/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

如果 BootLogo2 protocol 存在，并且显示的Logo图片个数大于0的话，就需要把显示的Logo信息上报了。当只有一个Logo图片的话，上报的Blt数据只是这个Logo图片的。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7sTG47jDsmDib2y3NwWOJLp5GhS84gjOvsFkFa0Y2Rnia15Z8ohI7icZ6Q/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

如果有多个Logo图片显示的话，之前的逻辑已经记录了所有Logo图片所占用的空间，基于占用空间的位置信息，合并后的Logo图片Blt数据通过GOP->Blt()函数的BltVideoToBltBuffer操作从屏幕上获取。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7WUVxlibicMmQDM3LxYyn7icbTIyLtAa3NxvNXoKdVIXyUpbWyl4QgiaJBw/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

这个函数的最后一步是把显示的Logo数据上报到BootLogo2 Protocol，BootLogo2 Protocol最终把Logo数据上报到ACPI BGRT表。

  

BootLogoEnableLogo()函数从PlatformLogo Protocol中获取显示的Logo图片，然后调用GOP Protocol把图片显示在屏幕上，最后把显示的图片信息上报给BootLogo2 Protocol。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7BXM53TLIcraqYqy4jAvBpPic9xbC7icicuEzic2j0r3RL7cPaCxUrnE2Sg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

BootLogoUpdateProgress () 是在屏幕上显示进度条，输入参数有点多，但是也不复杂。前三个参数是进度条的标题和颜色，后三个参数是进度条的进度值和颜色。它通常是在平台的PlatformBootManagerLib库中的PlatformBootManagerWaitCallback()函数中被调用，下一个章节的PlatformBmLib模块中会介绍它是如何被调用的。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7JR8TA25DkP4MF5ccTnqFUSQicdpHMDwhmkI1JO0RgHDhooP7oxTt2ag/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

首先也是找GOP protocol，从而获取当前屏幕的分辨率，单个进度块的宽度占屏幕宽度的百分之一；进度块的高度占屏幕高度的五十分之一。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7at3NKQolM90fDN2LtUpz7aV2zia6v5gDZGR93QPGJdCeFfU8AFYUbYg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

进度条的起始坐标X轴是0，Y轴是从屏幕的底端往上两个进度块的高度，也就是进度条是显示在屏幕的底端。当进度是0的时候，开始绘制进度条之前，需要把进度条所在的区间（最底端的一块空间）重置成Color（背景色），这个Color的值是全零（黑色），这里是写死的，更好的做法是通过函数的参数传进来，由调用的地方来决定进度条的背景色。在重置显示区间颜色的时候，使用的GOP操作是BltVideoFill，直接填充颜色。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ79nxiatuRH8Jy8GIibHBZ3UTcTYQV0FyuwJ029aPy02FcrnQ8jpq5xhYg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

基于输入的进度值，把当前的进度值和上一次的进度值之间的区间进行颜色填充，颜色是输入的进度条颜色，并且在填充的时候，进度块的宽度故意减1，留一个小缝隙不填充，这样显示出来的效果就是不连续的，见图一，如果希望是完全填充的效果，不要修改进度块的宽度，显示出来的效果见图四。最后调用PrintXY() API居中显示进度条的标题，PrintXY() API是MdePkg中的UefiLib提供的，它是用于在屏幕上显示字符串，前两个参数是显示的起始坐标，中间两个参数是字符串的前景色和背景色，最后一个参数是要显示的字符串。

  

BootLogoUpdateProgress()函数先是获取屏幕的分辨率，确定单个进度块的高度和宽度；然后在进度为0的时候，把进度条的显示区间填充为默认的黑色，最后是基于进度值填充进度条，并打印进度条的标题。

2

需求定制

- **ACPI BGRT表**
    

BGRT表是启动显示资源表，从ACPI规范5.0开始引入，解决的问题是BIOS的启动Logo在OS启动过程中也要一直生效，直到进入OS都是同一个启动Logo。BGRT表包含了显示图片的状态（有效和无效）、图片数据和图片的起始位置（X轴和Y轴），图片数据是只支持BMP格式，需要把GOP的BLT的数据转成BMP格式。BGRT表是由BootGraphicsResourceTableDxe模块安装的，这个模块从BootLogoEnableLogo()函数中接收到显示图片的BLT数据，在ReadyToBootEvent的CallBack函数里把BLT数据转成BMP格式，然后上报BGRT表。BootGraphicsResourceTableDxe是一个通用模块，平台只需要把这个模块添加到DSC/FDF里，就可以使能这个功能。下图（图五）是使用UEFI Shell的AcpiView命令显示已经安装的BGRT表。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7fkE8s6ZCdJagrmC1I7V18Rc9ibKJzmxIKlL1rZr8WsIpRWbNeYicicuHQ/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

图五：ACPI BGRT表

Status 是BGRT表的状态值，1是操作系统（OS）需要显示；0是不显示。Image Type只有0，表示是BMP格式的图片。图片的数据存在Image Address指向的内存空间。

- **进度条显示效果**
    

图三和图四是两种不同的进度条显示效果，一种是带分隔线的，另一种是完全填充的。如果需要定制显示效果，比如椭圆形的进度条，或者渐变色的进度条，修改BootLogoUpdateProgress()函数中GOP的调用方式，绘制不同的显示效果。

**PlatformBmLib 模块**

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7s2ZkpwWibKNLeTaORCJX1AlhQYcFWvcnQQ2mTEsXNLzFxapH4mYvSVQ/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

1

源码分析

这里以Edk2 EmulatorPkg的PlatformBmLib为例，说明Logo相关的显示函数是如何使用的。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7c033aEJuBq7GzCR8Ipr7aibDCRnbJ6Lubjf3bku2djcuHxrFrfX5Ldw/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

基于QuietBoot的值，决定是否需要调用 BootLogoEnableLogo() 函数显示Logo。实际上这里的逻辑是错误的，QuietBoot是静默启动的标记，静默启动是启动过程中不在屏幕上显示信息。不过这里的逻辑只是一个参考，告诉我们平台是可以控制是否显示Logo的。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ73xj1s65lK9dSRiaSYlOIw5G1qAQK1LojGSJbGkmVGibAuoh9mwaAsHicg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

调用PrintXY 在屏幕上打印提示信息，起始坐标X轴是10，Y轴是10，位与屏幕的左上方，前景色是白色，背景色是黑色，显示的字串是白色，最后一个参数是显示的字符串。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7LJliawy4HXr8FkOzn1shnYKLGwx7cCwV5ib1fqGmNMXhLfeexiaUVkF4Q/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

PlatformBootManagerAfterConsole () 在启动模式是RECOVERY_MODE的时候，不显示Logo，因为这个启动模式不会进入操作系统，不需要显示Logo。

  

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7cssHgyQPQrTFR4D62emiaQicTyjSWkrA2sNnhCecvqnbEW6ruCFtjNFg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

PlatformBootManagerWaitCallback()是PlatformBootManagerLib中对外的接口函数，输入参数是剩余等待时间（单位秒）。这个函数被BDS模块使用，在等待启动的过程中，每隔一秒会被定时调用。每次调用，输入参数的值TimeoutRemain会减1，TimeOut是需要等待的总时间，两者相减是已经等待的时间，乘以100再除以总时间，得到按百分比计算的进度值，最后调用BootLogoUpdateProgress()函数显示启动等待的进度条。进度条的前景色是白色，进度条的标题是”Start Boot option”，前景色是白色，背景色是黑色，上一次的进度值总是0。上一次的进度值是可以使用全局变量记录下来的，不应该总是0的。

2

需求定制

- **静默启动**
    

静默启动（Quiet Boot），启动的过程中屏幕上不显示任何信息，Logo、提示信息和启动进度条都不显示。静默启动一般是在特定的启动模式下需要的，比如RECOVERY启动时可以是静默启动。PlatformBmLib中需要基于静默启动的标记，决定是否显示Logo，打印提示信息和启动进度条。

- **进度条百分比显示**
    

百分比的显示方式比进度条更具体，所以也有需求是显示百分比，或者百分比和进度条同时显示，显示效果见图四。百分比的显示是在标题里，只要每次显示的时候把当前的进度值输出到标题字符串里。MdePkg PrintLib中的UnicodeSPrint()函数可以用于在字符串中记录进度值，这里需要注意的是使用%%来显示百分比字符%。

- **透明字体**
    

在使用全屏Logo时，屏幕上的提示信息往往会破坏图片的显示效果（见图二），这是由于字符串的背景色覆盖了原本的图片。为了解决这个问题，UEFI在显示字体的时候，定义了属性EFI_HII_OUT_FLAG_TRANSPARENT，透明显示只显示字符串的前景色，不显示字符串的背景色。字符显示的逻辑都在HiiDataBase模块中的HiiFont Protocol里实现。下面的代码片段是MdePkg UefiLib中PrintXY（）函数的实现，它调用HiiFont中StringToImage函数把字符显示在屏幕上，需要透明显示的话，可以把EFI_HII_OUT_FLAG_TRANSPARENT加入到输入的属性里。目前开源Edk2的HiiDataBase模块有一个Bug，字符串的背景色总是绘制，导致透明标记无法生效。我们已经解决了这个问题，透明字体的显示效果见图三。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7ucYHh8ZiacUddicC7rDbjaCgh6NYhKUp2hpsNMtquJTeRkApNQm64uhg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

  

**问题思考和本文代码链接**

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7s2ZkpwWibKNLeTaORCJX1AlhQYcFWvcnQQ2mTEsXNLzFxapH4mYvSVQ/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

最后留了三个小问题，请大家结合本文的讲解，提供解决思路和方案。也欢迎大家留言，提出开发过程中遇到的问题，我们一起来探讨研究。

1. 用户替换图片以后，无法正常显示，原因有哪些呢？
    
2. BGRT表是上报给操作系统，是否也可以把屏幕显示的内容保存成图片呢？
    
3. 现有的进度条是打印到屏幕上的，如何在串口设备上显示进度条呢？
    
      
    

**本文代码提取链接**：

https://pan.baidu.com/s/1rL1DBW3OI-zaGFa77lVfUg?pwd=logo

提取码：logo

**参考资料**

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/b1VnSPAicLiaJVn9Iiciaib3ty7I5r9nwnCQ7s2ZkpwWibKNLeTaORCJX1AlhQYcFWvcnQQ2mTEsXNLzFxapH4mYvSVQ/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

【1】https://uefi.org/

【2】https://github.com/tianocore/edk2

【3】https://github.com/lvandeve/lodepng

【4】https://github.com/cefqrn/jpeg-decoder

  

