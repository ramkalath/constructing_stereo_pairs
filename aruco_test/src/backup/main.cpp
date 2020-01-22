#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <vector>

#define markersize 200

int main()
{
	cv::Mat markerImage;
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	cv::aruco::drawMarker(dictionary, 23, markersize, markerImage, 1);
	cv::imwrite("marker23.png", markerImage);   return 0;

	//cv::VideoCapture cap(0);
	//cv::Mat frame;
	//std::vector<int> ids;

	////selecting predefined aruco dictionary
	//cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	//std::vector<std::vector<cv::Point2f>> corners;
	
	//while(1) 
	//{
		//cap >> frame;
		//if(frame.empty())
			//break;

		//cv::aruco::detectMarkers(frame, dictionary, corners, ids);
		//// if at least one marker detected
		//if (ids.size() > 0)
			//cv::aruco::drawDetectedMarkers(frame, corners, ids);

		//cv::imshow("", frame);
		//unsigned int key = cv::waitKey(20);
		//if(key==27)
			//break;
	//}



}


/**
 */
