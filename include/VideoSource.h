#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoProcessNode.h"

#ifndef VIDEOSOURCE
#define VIDEOSOURCE

using namespace std;
using namespace cv;


class VideoSource : public VideoProcessNode {
  public:
    VideoSource(string name, string filename, int loops,
            double start, double end, double speed,
            double sink_fps);
    virtual Mat nextFrame(void);
    virtual void createSliders(string window_name);
    
    string name;
    string filename;
    VideoCapture cap;
    
    static void updateSpeed(int new_speed, void *usrdata);
    static int max_speed;
    
  private:
  
    double current_time;
    int loops;
    double start; int start_int;
    double end; int end_int;
    double speed; int speed_int;
    double source_fps;
    double sink_fps;
    double num_source_frames;
};

#endif // VIDEOSOURCE

