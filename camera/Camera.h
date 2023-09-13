//
// Created by lining on 2023/9/12.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/opencv.hpp"

using namespace cv;

class Camera {

private:
    int _index;
public:
    int getIndex() const;

private:
    bool _isOpen = false;
    VideoCapture *_capture = nullptr;
    int _width = 0;
    int _height = 0;
    int _fps = 0;
public:
    Camera(int index);

    ~Camera();

public:

    bool IsOpen();

    int Open();

    int Close();

public:
    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

    int getFps() const;

    void setFps(int fps);

public:
    int GetOnePicture(Mat &frame);
};


#endif //CAMERA_H
