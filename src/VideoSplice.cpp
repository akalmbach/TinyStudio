#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

#include "VideoSplice.h"

using namespace std;
using namespace cv;

VideoSplice::VideoSplice(string name) : VideoProcessNode(name)
{
    this->name = name;
    frame_no = 0;
    pattern_length = 0;
        
    size = Size(0, 0);
}

Mat VideoSplice::nextFrame(void)
{
    if (size.height <= 0 || size.width <= 0) {
        setSizeFromSmallest();
    }
    if (pattern_length <= 0)
    {
        for (uint i = 0; i < children.size(); i++)
        {
            burst_sizes.push_back(1);
        }

        pattern_length = 0;
        for (uint i = 0; i < burst_sizes.size(); i++)
            pattern_length += burst_sizes[i];
    }
    
    if (children.size() > 0)
    {
        int pattern_pos = frame_no%pattern_length;
        int cur_child_idx = 0;
        for (uint i = 0; i < burst_sizes.size(); i++)
        {
            if (pattern_pos <= 0) 
            {
                cur_child_idx = i;
                break;
            }
            else
                pattern_pos -= burst_sizes[i];
        }
        Mat child_frame;
        Mat child_frame_resized = Mat::zeros(size, CV_8UC3);
        child_frame = children[cur_child_idx]->nextFrame();
        if (!child_frame.empty())
        {
            resize(child_frame, child_frame_resized, child_frame_resized.size(), 0, 0);
            frame_no++;
            return child_frame_resized;
        }
        else
        {
            children.erase(children.begin()+cur_child_idx);
            pattern_length -= burst_sizes[cur_child_idx];
            burst_sizes.erase(burst_sizes.begin()+cur_child_idx);
            return this->nextFrame();
        }
    }
    else
    {
        Mat empty_frame;
        return empty_frame;
    }
}

void VideoSplice::createSliders(string window_name)
{
    cout << "Not yet implemented: splice name=" << name << endl;
}

