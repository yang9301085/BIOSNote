# 工程_Mermaid图表优化

**来源**: 笔录 2026-02-05《ELK / Mermaid 图表》

#Mermaid #图表 #ELK #文档

---

# Mermaid 图表优化与 ELK 概念

## 核心知识点

### ELK 是什么
ELK = Elasticsearch + Logstash + Kibana，日志采集与分析三件套：
- **Elasticsearch**：搜索引擎/存储
- **Logstash**：日志收集与预处理
- **Kibana**：可视化界面

### Mermaid 常见图表类型
- **流程图（flowchart）**：表示过程/决策流
- **类图（classDiagram）**：展示类及其关系（UML）
- **活动图（activityDiagram）**：描述工作流

### Mermaid 常见优化问题
1. **线条交叉**：用 `smoothing: curved` 或调整布局方向
2. **节点重叠**：设置 `spacingLeft/spacingRight` 或换用更宽的布局
3. **背景色**：用 `fill` 属性自定义节点背景
4. **文字被遮挡**：增加 `width` 或减少节点内文字

### Mermaid → SVG 流程
```bash
# 用 mermaid CLI 或在线渲染器
mermaid --input diagram.mmd --output diagram.svg
```

