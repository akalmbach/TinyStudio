#include <opencv/cv.h>
#include <opencv/highgui.h>

#ifndef VIDEOPROCESSNODE
#define VIDEOPROCESSNODE

using namespace std;
using namespace cv;

class VideoProcessNode {
  public:
    VideoProcessNode(string name);
    virtual Mat nextFrame(void);
    virtual void createSliders(string window_name);
    string name;
    
  private:
    vector<VideoProcessNode> children;
};

#endif // VIDEOPROCESSNODE

