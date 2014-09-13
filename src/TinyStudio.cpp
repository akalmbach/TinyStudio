#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

#include "VideoSource.h"
#include "VideoSink.h"


using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
    VideoSource source1 = VideoSource("test_source1", argv[1],
                     10, 20, 2, 30);
    VideoSource source2 = VideoSource("test_source2", argv[1],
                     10, 30, 4, 30);
    VideoSource source3 = VideoSource("test_source3", argv[1],
                     10, 30, 1, 30);                     
    
    VideoSink sink = VideoSink("test sink", argv[2], 0, 0);
    sink.children.push_back(&source1);
    sink.children.push_back(&source2);
    sink.children.push_back(&source3);
    
    Mat next = sink.nextFrame();
    namedWindow("test", CV_NORMAL);
    while (!next.empty()){
        imshow("test", next);
        next = sink.nextFrame();
        waitKey(5);
    }
    return 0;
        
}

