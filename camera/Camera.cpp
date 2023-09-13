//
// Created by lining on 2023/9/12.
//

#include "Camera.h"
#include <glog/logging.h>


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

    LOG(INFO) << "Camera " << _index << " open success";
    _width = _capture->get(CAP_PROP_FRAME_WIDTH);
    _height = _capture->get(CAP_PROP_FRAME_HEIGHT);
    _fps = _capture->get(CAP_PROP_FPS);

    LOG(INFO) << "Camera " << _index << " width: " << _width << " height: " << _height << " fps: " << _fps;


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

int Camera::getWidth() const {
    return _width;
}

void Camera::setWidth(int width) {
    if (!_isOpen) {
        return;
    }
    // 设置分辨率
    if (_capture->set(CAP_PROP_FRAME_WIDTH, width)) {
        _width = width;
    }
}

int Camera::getHeight() const {
    return _height;
}

void Camera::setHeight(int height) {
    if (!_isOpen) {
        return;
    }
    if (_capture->set(CAP_PROP_FRAME_HEIGHT, height)) {
        _height = height;
    }
}

int Camera::getFps() const {
    return _fps;
}

void Camera::setFps(int fps) {
    if (!_isOpen) {
        return;
    }
    if (_capture->set(CAP_PROP_FPS, fps)) {
        _fps = fps;
    }
}

int Camera::getIndex() const {
    return _index;
}

int Camera::GetOnePicture(Mat &frame) {
    if (!_isOpen) {
        return -1;
    }

    *_capture >> frame;

    return 0;
}
