#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

#include "SpliceTest.h"

using namespace std;
using namespace cv;

SpliceTest::SpliceTest(string fn1, string fn2, string fnout)
{
    filename1 = fn1;
    filename2 = fn2;
    outfilename = fnout;
    cap1.open(filename1);
    cap2.open(filename2);
    
    if (!cap1.isOpened() || !cap2.isOpened())
    {
        cout << "Capture failed to open. Aborting" << endl;
    }
    else
    {
        init(0, 0);
        if (writer.isOpened())
            run();
        else
            cout << "Writer did not open" << endl;
    }
}

void SpliceTest::init(int x_size, int y_size)
{
    if (x_size == 0 || y_size == 0)
    {
        int smaller_x_size;
        int smaller_y_size;
        Mat tmp1;
        cap1 >> tmp1;
        if (tmp1.empty())
        {
            cout << "Warning: can't get any frames from " << filename1 << endl;
            return;
        }
        smaller_x_size = tmp1.cols;
        smaller_y_size = tmp1.rows;
        Mat tmp2;
        cap1 >> tmp2;
        if (tmp2.empty())
        {
            cout << "Warning: can't get any frames from " << filename2 << endl;
            return;
        }
        if (tmp2.cols < smaller_x_size)
            smaller_x_size = tmp2.cols;
        if (tmp2.rows < smaller_y_size)
            smaller_y_size = tmp2.rows;
        x_size = smaller_x_size;
        y_size = smaller_y_size;
    }
    size = Size(x_size, y_size);
    writer.open(outfilename,
        CV_FOURCC('P', 'I', 'M', '1'),
        30, 
        size,
        1
    );
    namedWindow("SpliceTest", CV_NORMAL);
}

void SpliceTest::run(void)
{

    Mat cur_frame;
    Mat out_frame = Mat::zeros(size, CV_8UC3);
    int iteration = 1;
    while (1)
    {
        if (iteration%2==0)
            cap2 >> cur_frame;
        else
            cap1 >> cur_frame;
        if (!cur_frame.empty())
        {
            resize(cur_frame, out_frame, out_frame.size(), 0, 0);
            imshow("SpliceTest", out_frame);
            writer << out_frame;
            waitKey(10);
        }
        else return;
        iteration++;
    }
}

int main(int argc, char *argv[]){
    if (argc == 4)
        SpliceTest st = SpliceTest(argv[1], argv[2], argv[3]);
    else
    {
        cout << "You need to specify exactly two inputs and one output!" << endl;
        return -1;
    }
    return 0;
}

