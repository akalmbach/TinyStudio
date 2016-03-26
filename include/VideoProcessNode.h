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
    void recursiveSetInteractiveMode(string window_name);
    virtual void createSliders(string window_name);
    string name;
    vector<VideoProcessNode *> children;
    
  protected:
    void setSizeFromSmallest(void);
    Size size;
    bool preview;
    static int max_slider_value;

};

#endif // VIDEOPROCESSNODE

