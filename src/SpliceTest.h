#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

#ifndef SPLICETEST
#define SPLICETEST

using namespace std;
using namespace cv;

class SpliceTest {
  public:
    SpliceTest(string fn1, string fn2, string fnout);
    void init(int x_size, int y_size);
    void run(void);
    Size size;
    
  private:
    string filename1;
    string filename2;
    string outfilename;
    VideoCapture cap1;
    VideoCapture cap2;
    VideoWriter writer;
};
#endif // SPLICETEST

