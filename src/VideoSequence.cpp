#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

#include "VideoSequence.h"

using namespace std;
using namespace cv;

VideoSequence::VideoSequence(string name) : VideoProcessNode(name)
{
    this->name = name;
        
    size = Size(0, 0);
}

Mat VideoSequence::nextFrame(void)
{
    if (size.height <= 0 || size.width <= 0) {
        setSizeFromSmallest();
    }

    Mat out_frame;
    if (children.size() > 0) {
        out_frame = Mat::zeros(size, CV_8UC3);
        Mat child_frame = children[0]->nextFrame();
        if (child_frame.empty()) {
            children.erase(children.begin());
            child_frame = this->nextFrame();
        }
        if (!child_frame.empty())
            if (child_frame.size() != this->size)
                resize(child_frame, out_frame, out_frame.size(), 0, 0);
            else
                out_frame = child_frame;
    }
    
    return out_frame;
}

void VideoSequence::createSliders(string window_name)
{
    cout << "Not yet implemented: name=" << name << endl;
}

