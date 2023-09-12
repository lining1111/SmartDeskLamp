//
// Created by lining on 2023/9/12.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/videoio.hpp"

using namespace cv;

class Camera {

private:
    int _index;
    bool _isOpen = false;
    VideoCapture *_capture = nullptr;
public:
    Camera(int index);

    ~Camera();

public:

    bool IsOpen();

    int Open();

    int Close();

};


#endif //CAMERA_H
