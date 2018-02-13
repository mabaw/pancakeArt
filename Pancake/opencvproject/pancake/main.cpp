#include <iostream>
#include <string.h>
#include <time.h>

#include <core.hpp>
#include <contrib.hpp>
#include <highgui.hpp>

void main()
{
	cv::Mat image2 = cv::imread("input.jpg");
	cv::Mat image3;
	if( image2.cols > image2.rows )
	{
		int dp = image2.cols - image2.rows;
		cv::Mat(image2, cv::Rect(dp/2, 0, image2.rows, image2.rows)).copyTo(image3);
	}
	else
	{
		int dp = image2.rows- image2.cols;
		cv::Mat(image2, cv::Rect(0, dp/2, image2.cols, image2.cols)).copyTo(image3);
	}

	cv::Mat image;
	cv::resize(image3, image, cv::Size(400, 400));
	cv::Mat gryImg1;
	cv::Mat gryImg2;
	cv::Mat gryImg3;
	cv::Mat canImg;
	cv::GaussianBlur(image, image, cv::Size(5, 5), 0, 0);
	cv::Canny(image, canImg, 1000, 1500, 5);
	cv::cvtColor(image, gryImg1, CV_BGR2GRAY);
	cv::cvtColor(image, gryImg2, CV_BGR2GRAY);
	cv::threshold(gryImg1, gryImg1, 250, 255, CV_THRESH_BINARY);
	cv::threshold(gryImg2, gryImg2, 64, 255, CV_THRESH_BINARY);

	cv::Mat resultImg;
	resultImg.create(400, 400, CV_8UC3);
	resultImg.setTo(0);

	for(int i = 0; i < image.rows; i++)
	{
		for(int j = 0; j < image.cols; j++)
		{
			if( gryImg1.at<uchar>(i, j) > 128 )
				cv::circle(resultImg, cv::Point(j, i), 0, CV_RGB(0, 0, 0));
			else if( gryImg2.at<uchar>(i, j) > 128 )
				cv::circle(resultImg, cv::Point(j, i), 0, CV_RGB(255, 200, 50));
			else
				cv::circle(resultImg, cv::Point(j, i), 2, CV_RGB(100, 63, 15), -1);
		}
	}
	
	for(int i = 0; i < image.rows; i++)
	{
		for(int j = 0; j < image.cols; j++)
		{
			if( canImg.at<uchar>(i, j) > 128 )
				cv::circle(resultImg, cv::Point(j, i), 3, CV_RGB(63, 31, 15), -1);
		}
	}

	cv::Mat bgImg = cv::imread("bg.jpg");
	std::vector<cv::Point2f> p0;
	std::vector<cv::Point2f> p1;

	p0.push_back(cv::Point2f(0, 0));
	p0.push_back(cv::Point2f(400, 0));
	p0.push_back(cv::Point2f(400, 400));
	p0.push_back(cv::Point2f(0, 400));

	p1.push_back(cv::Point2f(320, 233));
	p1.push_back(cv::Point2f(544, 241));
	p1.push_back(cv::Point2f(564, 513));
	p1.push_back(cv::Point2f(266, 503));

	cv::Mat perImg;
	cv::Mat perMat = cv::getPerspectiveTransform(p0, p1);
	cv::Mat maskImg = cv::imread("mask.jpg", 0);
	std::cout << "perMat : " << perMat << std::endl;
	cv::warpPerspective(resultImg, perImg, perMat, cv::Size(bgImg.cols, bgImg.rows));
	cv::threshold(perImg, gryImg1, 1, 255, CV_THRESH_BINARY);
	bgImg.copyTo(resultImg);
	perImg.copyTo(resultImg, gryImg1);
	cv::threshold(maskImg, gryImg1, 250, 255, CV_THRESH_BINARY_INV);
	bgImg.copyTo(resultImg, gryImg1);
	//cv::imshow("resultImg", resultImg);
	//cv::imshow("gryImg1", gryImg1);
    cv::imwrite( "images/resultImg.jpg", resultImg );
    cv::imwrite( "images/gryImg1.jpg", gryImg1 );

	//cv::waitKey();

	return 0;
}