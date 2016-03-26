#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "VideoProcessNode.h"

#ifndef VIDEOSKETCH
#define VIDEOSKETCH

using namespace std;
using namespace cv;

class VideoSketch : public VideoProcessNode {
  public:
    VideoSketch(string name);
    VideoSketch(string name_, int num_pyr_levels_, int num_orientations_,
                         int max_level_offset_, int blob_thresh_, int line_thickness_);
    Mat nextFrame(void);
    void createSliders(string window_name);
    string name;

  private:    
    Mat sketch(Mat &src);
    void drawContourLines(const Mat& src, Mat& dst);
    void convGabor(const Mat& src, Mat& dst, Size ksize, double sigma, double theta,
                double lambd, double gamma, double psi);
    
    int num_pyr_levels;
    int num_orientations;
    int max_level_offset;
    int blob_thresh;
    int line_thickness;
    
};

#endif // VIDEOSKETCH

