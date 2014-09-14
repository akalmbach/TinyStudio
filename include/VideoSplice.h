#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoProcessNode.h"

#ifndef VIDEOSPLICE
#define VIDEOSPLICE

using namespace std;
using namespace cv;

class VideoSplice : public VideoProcessNode {
  public:
    VideoSplice(string name);
    Mat nextFrame(void);
    void createSliders(string window_name);
    string name;

  private:    
    int frame_no;
    int pattern_length;
    vector<int> burst_sizes;
};

#endif // VIDEOSPLICE

