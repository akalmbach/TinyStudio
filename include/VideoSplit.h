#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoProcessNode.h"

#ifndef VIDEOSPLIT
#define VIDEOSPLIT

using namespace std;
using namespace cv;

class VideoSplit : public VideoProcessNode {
  public:
    VideoSplit(string name, double x1, double y1,
                        double x2, double y2);
    Mat nextFrame(void);
    void createSliders(string window_name);
    string name;

  private:   
    double rowToPct(int row);
    double colToPct(int col);
    double m;
    double b;
};

#endif // VIDEOSPLIT

