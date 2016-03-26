#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

#include "VideoSource.h"
#include "VideoSink.h"
#include "VideoSplice.h"
#include "VideoSplit.h"
#include "VideoSequence.h"
#include "VideoSketch.h"

#include "pugixml.hpp"

#define OUTPUT_FRAME_RATE 30

using namespace std;
using namespace cv;

VideoSequence * createSequenceFromXMLNode(pugi::xml_node& xml_node)
{
    cout << "Creating a sequence" << endl;
    string name = xml_node.attribute("name").as_string();
    VideoSequence *sequence_node = new VideoSequence(name);
    return sequence_node;
}

VideoSketch * createSketchFromXMLNode(pugi::xml_node& xml_node)
{
    cout << "Creating a sketch" << endl;
    string name = xml_node.attribute("name").as_string();
   
    int num_pyr_levels = 4;
    int num_orientations = 9;
    int max_level_offset = 3;
    int blob_thresh = 200;
    int line_thickness = 1;
    
    VideoSketch *sketch_node = new VideoSketch(name, num_pyr_levels, num_orientations,
            max_level_offset, blob_thresh, line_thickness);
    return sketch_node;
}

VideoSplit * createSplitFromXMLNode(pugi::xml_node& xml_node)
{
    cout << "Creating a split" << endl;
    string name = xml_node.attribute("name").as_string();
    double x1 = xml_node.attribute("x1").as_double();
    double x2 = xml_node.attribute("x2").as_double();
    double y1 = xml_node.attribute("y1").as_double();
    double y2 = xml_node.attribute("y2").as_double();
    VideoSplit *split_node = new VideoSplit(name, x1, y1, x2, y2);
    return split_node;
}

VideoSplice * createSpliceFromXMLNode(pugi::xml_node& xml_node)
{
    cout << "Creating a splice" << endl;
    string name = xml_node.attribute("name").as_string();
    VideoSplice *splice_node = new VideoSplice(name);
    return splice_node;
}

VideoSink * createSinkFromXMLNode(pugi::xml_node& xml_node)
{
    cout << "Creating a sink" << endl;
    string name = xml_node.attribute("name").as_string();
    string filename = xml_node.attribute("filename").as_string();
    int cols = xml_node.attribute("cols").as_int();
    int rows = xml_node.attribute("rows").as_int();
    VideoSink *video_node = new VideoSink(name, filename, cols, rows);
    return video_node;
}

VideoSource * createSourceFromXMLNode(pugi::xml_node& xml_node)
{
    cout << "Creating a source" << endl;
    string name = xml_node.attribute("name").as_string();
    string filename = xml_node.attribute("filename").as_string();
    double start_time = xml_node.attribute("start").as_double();
    double end_time = xml_node.attribute("end").as_double();
    double rate = xml_node.attribute("rate").as_double();
    int loops = xml_node.attribute("loops").as_double();
    VideoSource *video_node = new VideoSource(name, filename, loops,
         start_time, end_time, rate, OUTPUT_FRAME_RATE);
     return video_node;
}

VideoProcessNode * recurse_buildVideoProcessTree(pugi::xml_node& xml_node)
{
    // Create this node depending on its type
    VideoProcessNode *video_node;
    cout << "creating video node for xml element " <<  xml_node.attribute("name").as_string() << endl;
    string video_node_type = string(xml_node.name());
    if (video_node_type == "video")
    {
        video_node = createSinkFromXMLNode(xml_node);
    }
    else if (video_node_type == "source")
    {
        video_node = createSourceFromXMLNode(xml_node);
    }
    else if (video_node_type == "splice")
    {
        video_node = createSpliceFromXMLNode(xml_node);
    }
    else if (video_node_type == "split")
    {
        video_node = createSplitFromXMLNode(xml_node);
    }
    else if (video_node_type == "sequence")
    {
        video_node = createSequenceFromXMLNode(xml_node);
    } 
    else if (video_node_type == "sketch")
    {
        video_node = createSketchFromXMLNode(xml_node);
    }//....//
    else
    {
        cout << "Warning. Something went wrong while parsing XML" << endl;
        return 0;
    }
    
    // Add this node's children recursively
    int i = 0;
    for (pugi::xml_node xml_child = xml_node.first_child(); xml_child; xml_child = xml_child.next_sibling())
    {
        VideoProcessNode *video_child = recurse_buildVideoProcessTree(xml_child);
        video_node->children.push_back(video_child);
        i++;
    }
    
    cout << "done creating video node " << xml_node.attribute("name").as_string() << " and it's descendents" << endl;
    return video_node;
}

int main(int argc, char *argv[]){
    
    pugi::xml_document doc;
    bool interactive = false;
    cout << argc << endl;
    if (!doc.load_file(argv[1]))
    {   
        cout << "Please enter a project file" << endl;
        return -1;
    }
    if (argc > 2)
    {
        if (string(argv[2]) == "interactive")
        {
            interactive = true;
        }
    }
    
    pugi::xml_node root_xml_node = doc.child("video");
    VideoProcessNode *root_video_node = recurse_buildVideoProcessTree(root_xml_node);
    
    if (root_video_node == 0)
    {
        cout << "Got a null pointer for root video node" << endl;
        return -1;
    }
    
    if (interactive)
    {
        namedWindow("controls", CV_NORMAL);
        root_video_node->recursiveSetInteractiveMode("controls");
        waitKey(0);
    }
        
    Mat next = root_video_node->nextFrame();
    cout << "Got first frame" << endl;
    namedWindow("output", CV_NORMAL);
    while (!next.empty()){
        imshow("output", next);
        next = root_video_node->nextFrame();
        waitKey(1);
    }

    return 0;
        
}

