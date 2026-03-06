 sksurfaces是什么类？

• SkSurfaces 不是类，是一个 命名空间（namespace）。

  - SkSurfaces::Raster(...)
  - SkSurfaces::WrapPixels(...)
  - SkSurfaces::Null(...)

  真正的画布对象类型是 SkSurface（类），SkSurfaces::Raster(...) 只是负责创建并返回它（sk_sp<SkSurface>）。

  所以你那行代码的意思就是：通过 SkSurfaces 提供的工厂函数，创建一个 CPU 光栅 SkSurface。

› sksurface是什么类？什么是CPU光栅

• SkSurface 是 Skia 里的“绘制目标”类。
  你可以把它理解成“画布背后的像素容器”，canvas = surface->getCanvas() 后，所有 draw... 都画到这个 surface 上。

  CPU 光栅 的意思是：
  - 像素存在内存里（RAM）
  - 绘制计算由 CPU 完成
  - 不依赖 GPU 上下文
  
  对应你现在的用法 SkSurfaces::Raster(...)，就是创建一个 CPU 光栅 surface，适合学习和离屏导出 PNG。
  相对地，GPU surface 会把绘制交给显卡（需要额外图形上下文配置）。

• “光栅（Raster）”本质上就是一张像素网格。

  - 屏幕/图片可以看成很多小格子（像素）组成。
  - “光栅化”就是把线、圆、路径这些几何图形，转换成“每个像素该是什么颜色”。

  为什么叫“光栅”：
  - 这个词来自早期显示技术里的“逐行扫描（raster scan）”，像扫一排排栅格一样，所以叫光栅。

  和你现在的 Skia 场景对应：
  - SkSurfaces::Raster(...) = 创建一个像素网格画布。
  - CPU 光栅 = 由 CPU 逐像素算颜色并写入内存。

  对比一下更直观：
  - 矢量（Vector）：存“圆心、半径、路径”等数学描述。
  - 光栅（Raster）：存“像素颜色结果”。

• SkColorSetRGB 是一个颜色构造函数，用来快速生成 SkColor。

  含义：
  - 参数 R/G/B 都是 0~255
  - 自动把透明度 A 设为 255（完全不透明）

  等价于：
  SkColorSetARGB(255, r, g, b)

  例如：
  SkColorSetRGB(255, 0, 0)   // 红色，不透明

› Returns color value from 8-bit component values, with alpha set
  fully opaque to 255.这个是他的解释，为什么用Alpha


• 因为在 Skia 里，颜色通常是 RGBA/ARGB 四通道，不是只有 RGB。
  Alpha（A）表示透明度：

  - A=255：完全不透明
  - A=0：完全透明

  所以 SkColorSetRGB(r,g,b) 虽然只让你传 RGB，但它内部还是要生成一个完整颜色值，就把 A 默认设成 255（完全不透明）。

  为什么必须有 Alpha：

  1. 渲染时要做图层混合（blend/composite）
  2. 半透明绘制非常常见
  3. 统一用四通道，API 和底层处理更一致

  SkColorSetARGB(a, r, g, b)

› 什么是图层混合，什么是四通道？

• 四通道：一个颜色由 4 个数表示。

  - R 红
  - G 绿
  - B 蓝
  - A 透明度（Alpha）

  所以叫 RGBA（或 ARGB）。

  图层混合：把“上层像素”画到“下层像素”时，按透明度和混合规则算出最终颜色。
  最常见是“正常混合”（source over）：

  - 上层不透明（A=255）基本盖住下层
  - 上层半透明（A=128）上下颜色按比例混合

  直观例子：
  红色半透明圆画在蓝色背景上，看到的是偏紫的结果，这就是图层混合。

› 图层混合有什么用？

• 图层混合的核心作用：把多个绘制结果“叠起来”得到自然的最终画面。

  常见用途：
  1. 透明效果
     半透明面板、玻璃感、水印都靠混合。
  2. 抗锯齿边缘
     线条边缘会有半透明像素，混合后看起来更平滑。
  3. 阴影和发光
     阴影本质是半透明深色层叠加到背景上。
  4. UI 叠层
     按钮、弹窗、遮罩、选中高亮都要和底层内容混合。
  5. 图片与特效
     multiply/screen/overlay 这类模式可做调色和视觉特效。

  如果没有图层混合，绘制结果会像“硬覆盖”，画面会很生硬。