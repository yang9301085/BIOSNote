# BIOS_UI_Skia图形库

**来源**: 笔录 2026-02-27/2026-03-02《Skia 图形开发》

#BIOS #BIOS-UI #Skia #C++ #图形

---

# Skia 图形库在 BIOS UI 中的应用

## 核心知识点

### 关键类
| 类 | 作用 |
|---|---|
| `SkSurface` | 绑定到屏幕缓冲区的绘图表面 |
| `SkCanvas` | 绘图上下文，接收 draw 命令 |
| `SkPaint` | 绘图属性（颜色、字体、抗锯齿）|
| `SkRect/SkPoint` | 几何基本类型 |

### 典型用法
```cpp
// 创建画布
auto surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(width, height));
SkCanvas* canvas = surface->getCanvas();

// 绘制
canvas->drawRect(rect, paint);          // 矩形
canvas->drawCircle(x, y, radius, paint); // 圆（风扇叶片）
canvas->drawText(text, x, y, paint);   // 文字
paint.setAntiAlias(true);                 // 抗锯齿
```

### 动画实现
- 使用定时器（`OnTimer`）周期性调用 `CViewManager::invalidate_view()` 触发重绘
- 旋转动画：`angle += delta; canvas->rotate(angle, pivotX, pivotY);`

### 常见问题
- 字体返回 null：未正确加载字体库或字体文件缺失
- 旋转中心不对：用 `canvas->rotate(angle, pivotX, pivotY)` 指定旋转中心

