//
// Created by lining on 2023/9/12.
//

#include "Camera.h"

Camera::Camera(int index) {
    _index = index;
    _isOpen = false;
}

Camera::~Camera() {
    if (_isOpen) {
        Close();
    }
}

bool Camera::IsOpen() {
    return _isOpen;
}

int Camera::Open() {
    if (_isOpen) {
        return 0;
    }

    _capture = new VideoCapture(_index);
    if (!_capture->isOpened()) {
        delete _capture;
        _capture = nullptr;
        return -1;
    }


    _isOpen = true;
    return 0;
}

int Camera::Close() {
    if (!_isOpen) {
        if (_capture != nullptr) {
            if (_capture->isOpened()) {
                _capture->release();
            }
            delete _capture;
            _capture = nullptr;
        }
    }
    _isOpen = false;

    return 0;
}
