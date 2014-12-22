#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

#include "VideoProcessNode.h"

using namespace std;
using namespace cv;

VideoProcessNode::VideoProcessNode(string name)
{
    this->name = name;
    preview = false;
}

Mat VideoProcessNode::nextFrame(void)
{
    cout << "Warning, this should probably be overriden!" << endl;
    // Returns an empty Mat if all children are finished
    Mat from_child, next;
    for (uint i = 0; i < children.size(); i++)
    {
        from_child = children[i]->nextFrame();
        if (from_child.empty())
            children.erase(children.begin()+i);
        else
            next = from_child;
    }
    return next;
}

void VideoProcessNode::setSizeFromSmallest(void) {
    int smallest_rows = -1;
    int smallest_cols = -1;
    for (uint i = 0; i < children.size(); i++) {
        Mat init_frame = children[i]->nextFrame();
        if (!init_frame.empty())
        {
            if (smallest_rows <= 0 || init_frame.rows < smallest_rows)
            {
                smallest_rows = init_frame.rows;
            }
            if (smallest_cols <= 0 || init_frame.cols < smallest_cols)
            {
                smallest_cols = init_frame.cols;
            }
        }
    }
    size = Size(smallest_cols, smallest_rows);
}

void VideoProcessNode::createSliders(string window_name)
{
    cout << "No sliders defined for VideoProcessNode name=" << name << endl;
}
