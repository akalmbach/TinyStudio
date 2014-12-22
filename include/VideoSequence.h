#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoProcessNode.h"

#ifndef VIDEOSEQUENCE
#define VIDEOSEQUENCE

using namespace std;
using namespace cv;

class VideoSequence : public VideoProcessNode {
  public:
    VideoSequence(string name);
    virtual Mat nextFrame(void);
    virtual void createSliders(string window_name);
    
    string name;
    
  private:
    void querySliders(void);
    
};

#endif // VIDEOSEQUENCE

