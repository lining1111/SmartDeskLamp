add_executable(ppocr_demo ocr_db_crnn.cc clipper.cpp cls_process.cc crnn_process.cc db_post_process.cc)
target_link_libraries(ppocr_demo paddle_api_shared ${OpenCV_LIBS})
