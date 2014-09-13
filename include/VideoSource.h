#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoProcessNode.h"

#ifndef VIDEOSOURCE
#define VIDEOSOURCE

using namespace std;
using namespace cv;

class VideoSource : public VideoProcessNode {
  public:
    VideoSource(string name, string filename,
            double start, double end, double speed,
            double sink_fps);
    virtual Mat nextFrame(void);
    virtual void createSliders(string window_name);
    
    string name;
    string filename;
    VideoCapture cap;
    
  private:
    double current_time;
    double start;
    double end;
    double speed;
    double source_fps;
    double sink_fps;
    double num_source_frames;
};

#endif // VIDEOSOURCE

