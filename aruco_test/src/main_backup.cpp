/*****************************************************************************
* Filename : main.cpp
* Date : 4-1-2020
* Author : G Uma
* Email : guma1986@gmail.com
* Breif Description : Aruco testing
* Detailed Description : https://docs.opencv.org/trunk/d5/dae/tutorial_aruco_detection.html
*****************************************************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	// Mat markerImage;
	// Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	// aruco::drawMarker(dictionary, 23, 200, markerImage, 1);
	// imwrite("marker23.png", markerImage);   return 0;

	cv::VideoCapture inputVideo;
	inputVideo.open(0);

	//selecting predefined aruco dictionary
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	
	while (inputVideo.grab()) {
	    cv::Mat image, imageCopy;
	    inputVideo.retrieve(image);
	    
	    image.copyTo(imageCopy);
	    std::vector<int> ids;
	    std::vector<std::vector<cv::Point2f> > corners;
	    cv::aruco::detectMarkers(image, dictionary, corners, ids);
	    // if at least one marker detected
	    if (ids.size() > 0)
	        cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
	    cv::imshow("out", imageCopy);
	    char key = (char) cv::waitKey(1);
	    if (key == 27)
	        break;
	}



}