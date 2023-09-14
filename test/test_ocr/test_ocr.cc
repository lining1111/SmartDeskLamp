// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include "paddle_api.h" // NOLINT
#include "paddle_place.h"
#include "ocr/pipeline.h"
#include <iostream>
int main(int argc, char **argv) {
    if (argc < 7) {
        std::cerr << "[ERROR] usage: "
                  << " ./ocr_db_crnn_demo det_model_file cls_model_file "
                     "rec_model_file image_path"
                     " output_image_path charactor_dict config\n";
        exit(1);
    }
    std::string det_model_file = argv[1];
    std::string rec_model_file = argv[2];
    std::string cls_model_file = argv[3];
    std::string img_path = argv[4];
    std::string output_img_path = argv[5];
    std::string dict_path = argv[6];
    std::string config_path = argv[7];
    std::string cPUPowerMode = "";
    int cPUThreadNum = 1;
    Pipeline *pipe =
            new Pipeline(det_model_file, cls_model_file, rec_model_file, cPUPowerMode,
                         cPUThreadNum, config_path, dict_path);
    pipe->Process(img_path, output_img_path);
    return 0;
}
