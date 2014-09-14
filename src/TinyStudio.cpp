#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

#include "VideoSource.h"
#include "VideoSink.h"

#include "pugixml.hpp"

#define OUTPUT_FRAME_RATE 30

using namespace std;
using namespace cv;

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
    } //....//
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
    if (!doc.load_file(argv[1])) return -1;
    
    pugi::xml_node root_xml_node = doc.child("video");
    VideoProcessNode *root_video_node = recurse_buildVideoProcessTree(root_xml_node);

    if (root_video_node == 0)
    {
        cout << "Got a null pointer for root video node" << endl;
        return -1;
    }
        
    Mat next = root_video_node->nextFrame();
    namedWindow("test", CV_NORMAL);
    while (!next.empty()){
        imshow("test", next);
        next = root_video_node->nextFrame();
        //waitKey(5);
    }

    return 0;
        
}

