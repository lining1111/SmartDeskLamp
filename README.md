## 智能台灯

### 20230911

    建立项目基础

### 20230912

    编写opencv的cmake代码

### 20230913

    引入paddlelite-2.10库，增加ocr识别

    PaddleModels文件夹存的是paddlelite-2.10的模型    
    下载模型
    https://aistudio.baidu.com/modelsdetail/17?modelId=17 下载 1.1的中文检测模型 3.8M大小的
    
    输入指令：
    paddle_lite_opt --model_file=ch_PP-OCRv3_det_infer/inference.pdmodel --param_file=./ch_PP-OCRv3_det_infer/inference.pdiparams --optimize_out=./ch_PP-OCRv3_det_infer/ch_PP-OCRv3_det_infer --valid_targets=x86  --optimize_out_type=naive_buffer
    打印结果
    Loading topology data from ch_PP-OCRv3_det_infer/inference.pdmodel
    Loading params data from ./ch_PP-OCRv3_det_infer/inference.pdiparams
    1. Model is successfully loaded!
    2. Model is optimized and saved into ./ch_PP-OCRv3_det_infer/ch_PP-OCRv3_det_infer.nb successfully

    目前只有det模型是双平台的

    