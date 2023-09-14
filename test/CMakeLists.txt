#ocr
include_directories(../ocr)
add_executable(test_ocr test_ocr/test_ocr.cc
        ../ocr/pipeline.cc
        ../ocr/utils.cc
        ../ocr/rec_process.cc
        ../ocr/det_process.cc
        ../ocr/clipper.cpp
        ../ocr/cls_process.cc
        ../ocr/db_post_process.cc)
target_link_libraries(test_ocr ${LIBS})

#face_detection
add_executable(face_detection face_detection/face_detection.cc)
target_link_libraries(face_detection ${LIBS})

#face_keypoints_detection
add_executable(face_keypoints_detection face_keypoints_detection/face_keypoints_detection.cc)
target_link_libraries(face_keypoints_detection ${LIBS})

#human_segmentation
add_executable(human_segmentation human_segmentation/human_segmentation.cc)
target_link_libraries(human_segmentation ${LIBS})

#image_classification
add_executable(image_classification image_classification/image_classification.cc)
target_link_libraries(image_classification ${LIBS})

#mask_detection
add_executable(mask_detection mask_detection/mask_detection.cc)
target_link_libraries(mask_detection ${LIBS})

#object_detection picodet_detection
add_executable(picodet_detection object_detection/picodet_detection/object_detection_demo.cc)
target_link_libraries(picodet_detection ${LIBS})

#object_detection ssd_mobilenetv1_detection
add_executable(ssd_mobilenetv1_detection object_detection/ssd_mobilenetv1_detection/ssd_mobilenetv1_detection.cc)
target_link_libraries(ssd_mobilenetv1_detection ${LIBS})

#object_detection yolov3_mobilenet_v3
add_executable(yolov3_mobilenet_v3 object_detection/yolov3_mobilenet_v3/yolov3_mobilenet_v3.cc)
target_link_libraries(yolov3_mobilenet_v3 ${LIBS})

#object_detection yolov5n_detection
add_executable(yolov5n_detection object_detection/yolov5n_detection/yolov5n_detection.cc)
target_link_libraries(yolov5n_detection ${LIBS})

#pose_detection
add_executable(pose_detection pose_detection/pose_detection_demo.cc)
target_link_libraries(pose_detection ${LIBS})

#PP_shitu
add_executable(PP_shitu PP_shitu/src/object_detector.cc
        PP_shitu/src/pipeline.cc
        PP_shitu/src/recognition.cc
        PP_shitu/src/utils.cc)

target_link_libraries(PP_shitu ${LIBS})
