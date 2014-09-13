#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

#include "VideoSource.h"

using namespace std;
using namespace cv;

VideoSource::VideoSource(string name, string filename,
                         double start, double end, double speed,
                         double sink_fps) : VideoProcessNode(name)
{
    this->filename = filename;
    this->start = start;
    this->end = end;
    this->speed = speed;
    
    current_time = start;
    
    cap.open(filename);
    if (!cap.isOpened())
        cout << "Warning, failed to open video file " << filename << endl;
    source_fps = cap.get(CV_CAP_PROP_FPS);
    if (source_fps <= 0 or source_fps != source_fps)
        source_fps = 30.0;
    num_source_frames = cap.get(CV_CAP_PROP_FRAME_COUNT);
    if (num_source_frames <= 0 or num_source_frames != num_source_frames)
        source_fps = -1;
    this->sink_fps = sink_fps;
}

Mat VideoSource::nextFrame(void)
{
    Mat next_frame;
    //cout << "current time: " << current_time << endl;
    double step_time = speed/source_fps;
    //cout << "step time: " << step_time << endl;
    if (end > start)
    {
        current_time += step_time;
        if (current_time >= end)
            return next_frame;
    }
    else
    {   
        current_time -= step_time;
        if (current_time <= end)
            return next_frame;
    }


    double next_frame_idx;
    if (num_source_frames > 0)
        next_frame_idx = (double) (((int)round(current_time*sink_fps))%(int)num_source_frames);
    else
        next_frame_idx = round(current_time*sink_fps);
    
    cap.set(CV_CAP_PROP_POS_FRAMES, next_frame_idx);
    cap >> next_frame;
    
    return next_frame;
}

void VideoSource::createSliders(string window_name)
{
    cout << "Not yet implemented: Source name=" << name << endl;
}

