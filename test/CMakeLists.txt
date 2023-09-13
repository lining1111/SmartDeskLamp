include_directories(../ocr)
add_executable(ppocr_demo test_ocr/ocr_db_crnn.cc
        ../ocr/clipper.cpp
        ../ocr/cls_process.cc
        ../ocr/crnn_process.cc
        ../ocr/db_post_process.cc)
target_link_libraries(ppocr_demo paddle_api_shared ${OpenCV_LIBS})