# 动手学大模型 Ch1 - 预训练语言模型微调与部署

> [!summary] 概述
> 介绍预训练模型微调与部署，掌握 Transformers 工具包，通过微调 BERT 实现虚假新闻分类，并部署 Gradio Demo。

## 本章目标

1. 熟悉使用 Transformers 工具包
2. 掌握预训练模型的微调、推理（解耦可定制版本 & 默认集成版本）
3. 掌握利用 Gradio Spaces 进行 Demo 部署
4. 了解不同类型的预训练模型的选型和应用场景

## 工具链

- **🤗 Transformers**: HuggingFace 核心库，支持 NLP/CV/Audio/多模态任务
  - 中文文档: https://huggingface.co/docs/transformers/main/zh/index
- **Gradio**: 快速构建 Web Demo
- **数据集**: Kaggle 虚假推文数据集 (nlp-getting-started)

## 环境安装

```bash
conda create -n llm python=3.9
conda activate llm
pip install transformers
# 删除 requirements.txt 中的 torch 后
pip install -r requirements.txt
conda install pytorch  # GPU 版本，不要用国内源会自动装 CPU 版
```

> [!warning] 网络问题
> - 模型下载失败 → 手动从 HuggingFace 下载到本地
> - evaluate 指标加载失败 → 从 GitHub 下载 evaluate 包到本地

## 两套实现版本

### 版本一：解耦可定制版本（推荐入门）

三个文件完全解耦，便于理解每一步：

| 文件 | 职责 |
|---|---|
| `utils_data.py` | 数据加载与预处理（CSV → Dataset） |
| `modeling_bert.py` | BERT 模型结构定义 + tokenizer 加载 |
| `main.py` | 训练循环 + 验证 + 预测 |

```bash
python main.py  # 一条命令跑完全流程
```

**核心流程**：
1. `utils_data.py` — 加载 CSV 数据集，文本清洗 + tokenization
2. `modeling_bert.py` — 加载 `bert-base-uncased`，接分类头（Linear Layer）
3. `main.py` — `torch.nn.CrossEntropyLoss` + 训练/验证/预测

### 版本二：默认集成版本（run_classification.py）

基于 HuggingFace 官方脚本，功能更全但代码复杂。

```bash
python run_classification.py \
    --model_name_or_path bert-base-uncased \
    --train_file data/train.csv \
    --validation_file data/val.csv \
    --test_file data/test.csv \
    --do_train --do_eval --do_predict \
    --max_seq_length 512 \
    --per_device_train_batch_size 32 \
    --learning_rate 2e-5 \
    --num_train_epochs 1 \
    --output_dir experiments/
```

**关键超参数**：
| 参数 | 含义 | 推荐值 |
|---|---|---|
| `--max_seq_length` | 最大序列长度 | 512 |
| `--per_device_train_batch_size` | batch size | 32 |
| `--learning_rate` | 学习率 | 2e-5 |
| `--num_train_epochs` | 训练轮数 | 1-3 |
| `--metric_name` | 评价指标 | accuracy |

**核心模块**：
1. **数据加载**: `AutoTokenizer` 处理文本，CSV/JSON 格式
2. **模型加载**: `AutoModelForSequenceClassification` 自动选带分类头的模型
3. **训练**: HuggingFace `Trainer` 封装完整训练循环
4. **评估**: 在 dev set 上评估，test set 上预测

## Gradio Spaces 部署

```
训练好的 checkpoint → 创建 HuggingFace Space (Gradio SDK) → 上传 app.py + requirements.txt → 在线 Demo
```

**`app.py` 核心结构**：
```python
import gradio as gr
from transformers import pipeline

classifier = pipeline("text-classification",
                     model="cooelf/text-classification",
                     top_k=None)

interface = gr.Interface(fn=predict, inputs="text", outputs="label")
interface.launch()
```

**requirements.txt**：
```
transformers==4.30.2
torch==2.0.0
```

部署案例：https://huggingface.co/spaces/cooelf/text-classification

## 模型选型参考

| 任务类型 | 推荐模型 |
|---|---|
| 文本分类 | BERT-base, RoBERTa-base |
| 问答 | BERT, ALBERT |
| 文本生成 | GPT-2, Llama2 |
| 多语言 | mBERT, XLM-RoBERTa |
| 轻量微调 | Llama2 + LoRA (PEFT) |

## 延伸资源

- 问答模型: `transformers/examples/pytorch/question-answering`
- 文本摘要: `transformers/examples/pytorch/summarization`
- Llama2 推理: https://huggingface.co/docs/transformers/en/model_doc/llama2
- LoRA 微调: https://github.com/peremartra/Large-Language-Model-Notebooks-Course
- 综述论文: [Large Language Models: A Survey](https://arxiv.org/pdf/2402.06196.pdf) (Word2Vec 作者出品)

---

## 上手指南

1. 克隆仓库: `git clone https://github.com/yang9301085/dive-into-llms`
2. 进入 chapter1: `cd dive-into-llms/documents/chapter1`
3. 下载工程包（Google Drive 链接）和数据集（Kaggle）
4. 配置 conda 环境
5. 运行 `python main.py` 或 `python run_classification.py`
6. 部署到 Gradio Spaces

> [!tip] 本地模型下载
> 若网络不通，先用代理下载: `export HF_ENDPOINT=https://hf-mirror.com`
> `huggingface-cli download --resume-download google-bert/bert-base-uncased --local-dir ./model/bert-base-uncased`

---

*由 Jarvis 整理，2026-03-25*
