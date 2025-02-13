//   Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
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

#include "utils.h"    // NOLINT

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h> // NOLINT
#endif
namespace PPShiTu {

    void nms(std::vector<ObjectResult> *input_boxes, float nms_threshold,
             bool rec_nms) {
        if (!rec_nms) {
            std::sort(input_boxes->begin(), input_boxes->end(),
                      [](ObjectResult a, ObjectResult b) {
                          return a.confidence > b.confidence;
                      });
        } else {
            std::sort(input_boxes->begin(), input_boxes->end(),
                      [](ObjectResult a, ObjectResult b) {
                          return a.rec_result[0].score > b.rec_result[0].score;
                      });
        }
        std::vector<float> vArea(input_boxes->size());
        for (int i = 0; i < input_boxes->size(); ++i) {
            vArea[i] = (input_boxes->at(i).rect[2] - input_boxes->at(i).rect[0] + 1) *
                       (input_boxes->at(i).rect[3] - input_boxes->at(i).rect[1] + 1);
        }
        for (int i = 0; i < input_boxes->size(); ++i) {
            for (int j = i + 1; j < input_boxes->size();) {
                float xx1 =
                        std::max(input_boxes->at(i).rect[0], input_boxes->at(j).rect[0]);
                float yy1 =
                        std::max(input_boxes->at(i).rect[1], input_boxes->at(j).rect[1]);
                float xx2 =
                        std::min(input_boxes->at(i).rect[2], input_boxes->at(j).rect[2]);
                float yy2 =
                        std::min(input_boxes->at(i).rect[3], input_boxes->at(j).rect[3]);
                float w = std::max(0.f, xx2 - xx1 + 1);
                float h = std::max(0.f, yy2 - yy1 + 1);
                float inter = w * h;
                float ovr = inter / (vArea[i] + vArea[j] - inter);
                if (ovr >= nms_threshold) {
                    input_boxes->erase(input_boxes->begin() + j);
                    vArea.erase(vArea.begin() + j);
                } else {
                    j++;
                }
            }
        }
    }

// fill tensor with mean and scale and trans layout: nhwc -> nchw, neon speed up
    void neon_mean_scale(const float *din, float *dout, int size, float *mean,
                         float *scale) {
        float *dout_c0 = dout;
        float *dout_c1 = dout + size;
        float *dout_c2 = dout + size * 2;

        int i = 0;
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        float32x4_t vmean0 = vdupq_n_f32(mean[0]);
        float32x4_t vmean1 = vdupq_n_f32(mean[1]);
        float32x4_t vmean2 = vdupq_n_f32(mean[2]);
        float32x4_t vscale0 = vdupq_n_f32(1.f / scale[0]);
        float32x4_t vscale1 = vdupq_n_f32(1.f / scale[1]);
        float32x4_t vscale2 = vdupq_n_f32(1.f / scale[2]);

        for (; i < size - 3; i += 4) {
            float32x4x3_t vin3 = vld3q_f32(din);
            float32x4_t vsub0 = vsubq_f32(vin3.val[0], vmean0);
            float32x4_t vsub1 = vsubq_f32(vin3.val[1], vmean1);
            float32x4_t vsub2 = vsubq_f32(vin3.val[2], vmean2);
            float32x4_t vs0 = vmulq_f32(vsub0, vscale0);
            float32x4_t vs1 = vmulq_f32(vsub1, vscale1);
            float32x4_t vs2 = vmulq_f32(vsub2, vscale2);
            vst1q_f32(dout_c0, vs0);
            vst1q_f32(dout_c1, vs1);
            vst1q_f32(dout_c2, vs2);

            din += 12;
            dout_c0 += 4;
            dout_c1 += 4;
            dout_c2 += 4;
        }
#endif
        for (; i < size; i++) {
            *(dout_c0++) = (*(din++) - mean[0]) / scale[0];
            *(dout_c0++) = (*(din++) - mean[1]) / scale[1];
            *(dout_c0++) = (*(din++) - mean[2]) / scale[2];
        }
    }

    float fast_exp(float x) {
        union {
            uint32_t i;
            float f;
        } v{};
        v.i = (1 << 23) * (1.4426950409 * x + 126.93490512f);
        return v.f;
    }

    void activation_function_softmax(const float *src, float *dst, int length) {
        const float alpha = *std::max_element(src, src + length);
        float denominator{0.f};

        for (int i = 0; i < length; ++i) {
            dst[i] = fast_exp(src[i] - alpha);
            denominator += dst[i];
        }
        for (int i = 0; i < length; ++i) {
            dst[i] /= denominator;
        }
    }

} // NOLINT
