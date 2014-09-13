#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

#include "VideoSink.h"

using namespace std;
using namespace cv;

VideoSink::VideoSink(string name, string filename, 
                     int cols, int rows)
    : VideoProcessNode(name)
{
    this->filename = filename;
    size = Size(cols, rows);
}

void VideoSink::setSizeFromSmallest(void) {
    int smallest_rows = -1;
    int smallest_cols = -1;
    for (uint i = 0; i < children.size(); i++) {
        Mat init_frame = children[i]->nextFrame();
        if (smallest_rows <= 0 || init_frame.rows < smallest_rows)
            smallest_rows = init_frame.rows;
        if (smallest_cols <= 0 || init_frame.cols < smallest_cols)
            smallest_cols = init_frame.cols;
    }
    size = Size(smallest_cols, smallest_rows);
}

Mat VideoSink::nextFrame(void)
{
    if (size.height <= 0 || size.width <= 0) {
        setSizeFromSmallest();
    }
    if (!writer.isOpened())
    {
        writer.open(filename,
            CV_FOURCC('P', 'I', 'M', '1'), // Have to experiment with FOURCCss
            30, // Frame rate
            size,
            1 // Output in color
        );
    }
    
    Mat out_frame = Mat::zeros(size, CV_8UC3);
    for (uint i = 0; i < children.size(); i++) {
        Mat child_frame;
        Mat child_frame_resized = Mat::zeros(size, CV_8UC3);
        child_frame = children[i]->nextFrame();
        if (!child_frame.empty())
        {
            child_frame /= children.size();
            resize(child_frame, child_frame_resized, out_frame.size(), 0, 0);
            out_frame += child_frame_resized;
        }
        else
        {
            cout << name << "Child " << children[i]->name << " finished." << endl;
            children.erase(children.begin()+i);
        }
    }
    
    if (writer.isOpened() && children.size() > 0)
    {
        writer << out_frame;
        return out_frame;
    }
    else
    {
        Mat empty_frame;
        return empty_frame;
    }
}

void VideoSink::createSliders(string window_name)
{
    cout << "Not yet implemented: Sink name=" << name << endl;
}

