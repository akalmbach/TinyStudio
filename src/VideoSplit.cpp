#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

#include "VideoSplit.h"

using namespace std;
using namespace cv;

VideoSplit::VideoSplit(string name, double x1, double y1,
                        double x2, double y2) : VideoProcessNode(name)
{
    this->name = name;
    this->m = (y1-y2)/(x1-x2);
    this->b = y1 - m*x1;
        
    size = Size(0, 0);
}

double VideoSplit::rowToPct(int row)
{
    return (double)row/(double)size.height;
}

double VideoSplit::colToPct(int col)
{
    return (double)col/(double)size.width;
}

Mat VideoSplit::nextFrame(void)
{
    if (size.height <= 0 || size.width <= 0) {
        setSizeFromSmallest();
    }
    Mat out_frame;
    
    if (children.size() >= 2)
    {
        out_frame = Mat::zeros(size, CV_8UC3);
        Mat in_frame0_resized = Mat::zeros(size, CV_8UC3);
        Mat in_frame1_resized = Mat::zeros(size, CV_8UC3);
        Mat in_frame0 = children[0]->nextFrame();
        Mat in_frame1 = children[1]->nextFrame();
        if (in_frame0.empty() && in_frame1.empty())
        {
            // end here when both inputs are finished
            return in_frame0;
        }
        if (!in_frame0.empty())
            resize(in_frame0, in_frame0_resized, in_frame0_resized.size(), 0, 0);
        if (!in_frame1.empty())
            resize(in_frame1, in_frame1_resized, in_frame1_resized.size(), 0, 0);
            
        for (int row = 0; row < out_frame.rows; row++) {
            for (int col = 0; col < out_frame.cols; col++) {
                if (rowToPct(row) > m*colToPct(col) + b)
                    out_frame.at<Vec3b>(row, col) = in_frame0_resized.at<Vec3b>(row, col);
                else
                    out_frame.at<Vec3b>(row, col) = in_frame1_resized.at<Vec3b>(row, col);
            }
        }
        return out_frame;
    }
    else{
        Mat empty_frame;
        return empty_frame;
    }
}

void VideoSplit::createSliders(string window_name)
{
    cout << "Not yet implemented: splice name=" << name << endl;
}

