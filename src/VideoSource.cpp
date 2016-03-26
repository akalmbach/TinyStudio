#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

#include "VideoSource.h"

using namespace std;
using namespace cv;

int VideoSource::max_speed = 4;

VideoSource::VideoSource(string name, string filename, int loops,
                         double start, double end, double speed,
                         double sink_fps) : VideoProcessNode(name)
{
    this->loops = loops;
    this->filename = filename;
    this->start = start;
    this->end = end;
    this->speed = speed;
    this->name = name;
    
    current_time = start;
    
    cout << "Creating VideoSource (name=" << name;
    cout << ", filename=" << filename;
    cout << ", speed*[start, end]=" << speed << "*" << start << ", " << end;
    cout << endl;
    
    cap.open(filename);
    if (!cap.isOpened())
        cout << "Warning, failed to open video file " << filename << endl;
    source_fps = cap.get(CV_CAP_PROP_FPS);
    cout << "Got fps: " << source_fps << endl;
    
    if (source_fps <= 0 or source_fps != source_fps)
    {
        cout << "fps fallback --->" << endl;
        // try getting one frame and counting ms.
        double start_ms = cap.get(CV_CAP_PROP_POS_MSEC);
        Mat scratch_frame;
        cap >> scratch_frame;
        double end_ms = cap.get(CV_CAP_PROP_POS_MSEC);
        if (start_ms != start_ms || end_ms != end_ms || start_ms == end_ms)
        {
            cout << "   ...Fail to increment video time.";
            cout << "   ...Defaulting to 30 fps" << endl;
            source_fps = 30.0;
        }
        else
        {
            double d_ms = end_ms - start_ms;
            source_fps = 1000.0/d_ms;
            cout << "   ...Frame step size ms: " << d_ms << endl;
        } 
    }
    
    cout << ". Using fps: " << source_fps;
    
    num_source_frames = cap.get(CV_CAP_PROP_FRAME_COUNT);
    
    if (num_source_frames <= 0 or num_source_frames != num_source_frames)
        source_fps = -1;
    cout << ". Num source frames: " << num_source_frames << endl;
    this->sink_fps = sink_fps;
    
    this->speed_int = (int) ((this->speed/VideoSource::max_speed) * VideoProcessNode::max_slider_value);
}

Mat VideoSource::nextFrame(void)
{
    Mat next_frame;
    cout << name << ": current time/total: " << current_time;
    cout << "/" << end << endl;
    double step_time = speed/sink_fps;
    //cout << ", step time: " << step_time << endl;
    if (end > start)
    {
        current_time += step_time;
        if (current_time >= end)
            if (loops > 0) {
                current_time = (current_time-end)+start;
                loops--;
            }
            else
                return next_frame;
    }
    else
    {   
        current_time -= step_time;
        if (current_time <= end)
            if (loops > 0) {
                current_time = start;
                loops--;
            }
            else
                return next_frame;
    }


    double next_frame_idx;
    if (num_source_frames > 0)
        next_frame_idx = (double) (((int)round(current_time*source_fps))%(int)num_source_frames);
    else
        next_frame_idx = round(current_time*source_fps);
    
    cap.set(CV_CAP_PROP_POS_FRAMES, next_frame_idx);
    cap >> next_frame;
    
    return next_frame;
}

void VideoSource::createSliders(string window_name)
{
    createTrackbar(name+"_speed", window_name, &speed_int, VideoProcessNode::max_slider_value, &VideoSource::updateSpeed, this);
}

void VideoSource::updateSpeed(int new_speed, void *usrdata)
{
    VideoSource *video_source = (VideoSource *) usrdata;
    
    double new_speed_f = (double) (new_speed*VideoSource::max_speed) / VideoProcessNode::max_slider_value;
    video_source->speed = new_speed_f;
    cout << "Set speed to: " << new_speed_f << "(" << video_source->name << ")" << endl;
}
