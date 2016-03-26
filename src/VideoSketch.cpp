#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

#include "VideoSketch.h"

using namespace std;
using namespace cv;

VideoSketch::VideoSketch(string name) : VideoProcessNode(name)
{
    this->name = name;
    
    // TODO - Sliderize
    int num_pyr_levels = 5;
    int num_orientations = 11;
    int max_level_offset = 4;
    int blob_thresh = 200;
    int thickness = 2;
        
    size = Size(0, 0);
}

VideoSketch::VideoSketch(string name_, int num_pyr_levels_, int num_orientations_,
                         int max_level_offset_, int blob_thresh_, int line_thickness_)
     : VideoProcessNode(name_)
{
    name = name_;
    num_pyr_levels = num_pyr_levels_;
    num_orientations = num_orientations_;
    max_level_offset = max_level_offset_;
    blob_thresh = blob_thresh_;
    line_thickness = line_thickness_;
        
    size = Size(0, 0);
}

Mat VideoSketch::nextFrame(void)
{
    if (size.height <= 0 || size.width <= 0) {
        setSizeFromSmallest();
    }
    
    if (children.size() > 0)
    {
        Mat child_frame;
        Mat child_frame_resized = Mat::zeros(size, CV_8UC3);
        child_frame = children[0]->nextFrame();
        if (!child_frame.empty())
        {
            resize(child_frame, child_frame_resized, child_frame_resized.size(), 0, 0);
            Mat sk = sketch(child_frame_resized);
            return sk;
        }
        else
        {
            children.erase(children.begin());
            return this->nextFrame();
        }
    }
    else
    {
        Mat empty_frame;
        return empty_frame;
    }
}

Mat VideoSketch::sketch(Mat &src)
{
    Mat src_gray, all_lines;
    cvtColor(src, src_gray, CV_BGR2GRAY);

    // ???
    src_gray.convertTo(src_gray, CV_32FC1, (1/255.0));
    
    // Initialize output
    src_gray.convertTo(all_lines, CV_8UC1, 255);
    all_lines = all_lines*0;
    
    int min_dim = min(src_gray.rows, src_gray.cols);
    int max_pyr_level = int(min(log2(min_dim) - max_level_offset, 9.0));
    
    int seq_no = 0;
    for (int pyr_level = max_pyr_level; pyr_level > max_pyr_level-num_pyr_levels; pyr_level--)
    {
        Mat gabor_img;
        double sigma = pow(2.0, double(pyr_level));
        int ksize = int(2*sigma);
        for (int orientation = 0; orientation < num_orientations; orientation ++)
        {
            // Convolve with Gabor Kernel
            double theta = (double(orientation)/double(num_orientations))*M_PI;
            
            convGabor(src_gray, gabor_img, Size(ksize, ksize), sigma, theta, 2*sigma, 0.5, M_PI);
            
            // Normalize the output
            double min_val, max_val;
            minMaxLoc(gabor_img, &min_val, &max_val);
            Mat gabor_img_normalized = (gabor_img-min_val)/(max_val-min_val);
            
            Mat lines, countours, blobs;
            drawContourLines(gabor_img, lines);
            
            all_lines += lines;
        }
    }
    
    Mat out;
    cvtColor(all_lines, out, CV_GRAY2BGR);
    
    return out;
}


void VideoSketch::drawContourLines(const Mat& src, Mat& dst)
{
    RNG rng(12345);
    Mat threshold_output;
    
    Mat src_8uc1;
    src.convertTo(src_8uc1, CV_8UC1, 255);

    /// Detect edges using Threshold
    threshold(src_8uc1, threshold_output, blob_thresh, 255, THRESH_BINARY );
    /// Find contours
    vector < vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Find the rotated rectangles and ellipses for each contour
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
    { 
        minRect[i] = minAreaRect( Mat(contours[i]) );
        if( contours[i].size() > 5 )
        { 
            minEllipse[i] = fitEllipse( Mat(contours[i]) ); 
        }
    }

    /// Draw contours + rotated rects + ellipses
    dst = Mat::zeros( threshold_output.size(), CV_8UC1 );
    Mat contour_img = Mat::zeros( threshold_output.size(), CV_8UC3 );
    
    vector< pair<Point2f, Point2f> > point_pairs;
    
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        // contour
        drawContours( contour_img, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        // ellipse
        ellipse( contour_img, minEllipse[i], color, 2, 8 );
        // rotated rectangle
        Point2f rect_points[4]; minRect[i].points( rect_points );
        for( int j = 0; j < 4; j++ )
            line( contour_img, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
        
        // TODO: Option to return contour img instead of line img
            
        Point2f center = minRect[i].center;
        Size2f size = minRect[i].size;
        float angle = minRect[i].angle*M_PI/180.0;
        while (angle < 0) angle += M_PI;
        while (angle > M_PI) angle -= M_PI;

        Point2f p, q;
        float maj_axis_size = max(size.height, size.width);
        float min_axis_size = min(size.height, size.width);
        if (size.height > size.width) angle -= M_PI/2;

        if (maj_axis_size/min_axis_size >= 3.0)
        {
            p.x = center.x + cos(angle)*maj_axis_size/2.0;
            p.y = center.y + sin(angle)*maj_axis_size/2.0;
            q.x = center.x - cos(angle)*maj_axis_size/2.0;
            q.y = center.y - sin(angle)*maj_axis_size/2.0;
            line(dst, p, q, Scalar::all(255), line_thickness);
        }
    }
}

void VideoSketch::convGabor(const Mat& src, Mat& dst,
              Size ksize, double sigma, double theta,
                double lambd, double gamma, double psi)
{
    Mat kernel = getGaborKernel(ksize, sigma, theta, lambd, gamma, psi, CV_32F);
    filter2D(src, dst, CV_32F, kernel);
}

void VideoSketch::createSliders(string window_name)
{
    cout << "Not yet implemented: Sketch name=" << name << endl;
}

