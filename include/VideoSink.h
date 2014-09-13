#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoProcessNode.h"

#ifndef VIDEOSINK
#define VIDEOSINK

using namespace std;
using namespace cv;

class VideoSink : public VideoProcessNode {
  public:
    VideoSink(string name, string filename, int rows, int cols);
    Mat nextFrame(void);
    void createSliders(string window_name);
    
    string name;
    string filename;
    VideoWriter writer;
    
  private:
    void setSizeFromSmallest(void);
    Size size;
};

#endif // VIDEOSOURCE

