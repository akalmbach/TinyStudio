#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

#include "VideoProcessNode.h"

using namespace std;
using namespace cv;

VideoProcessNode::VideoProcessNode(string name)
{
    this->name = name;
}

Mat VideoProcessNode::nextFrame(void)
{
    // Returns an empty Mat if all children are finished
    Mat from_child, next;
    for (uint i = 0; i < children.size(); i++)
    {
        from_child = children[i].nextFrame();
        if (from_child.empty())
            children.erase(children.begin()+i);
        else
            next = from_child;
    }
    return next;
}

void VideoProcessNode::createSliders(string window_name)
{
    cout << "No sliders defined for VideoProcessNode name=" << name << endl;
}
