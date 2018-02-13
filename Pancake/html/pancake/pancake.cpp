#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "ext/standard/info.h"


#include "php_pancake.h"


#include <iostream>
#include <string.h>
#include <time.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
//#include <core.h>
//#include <contrib.hpp>
//#include <highgui.hpp>

using namespace cv;

static zend_function_entry pancake_functions[] = {
    PHP_FE(to_pancake, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry pancake_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_PANCAKE_EXTNAME,
    pancake_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_MINFO(pancake),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_PANCAKE_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PANCAKE
extern "C" {
	ZEND_GET_MODULE(pancake)
}
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINFO_FUNCTION(pancake)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "pancake support", "enabled");
	php_info_print_table_row(2, "pancake version", PHP_PANCAKE_VERSION);
	php_info_print_table_row(2, "OpenCV version", CV_VERSION);
	php_info_print_table_end();
}
/* }}} */

static void php_pancake(INTERNAL_FUNCTION_PARAMETERS, int return_type)
{
	char *file, *casc;
	char *outfile;
	long flen, clen, olen;

	if(zend_parse_parameters(3 TSRMLS_CC, "sss", &file, &flen, &outfile, &olen, &casc, &clen) == FAILURE) {
		RETURN_NULL();
	}



	Mat image2 = imread(file);

	if (!image2.data)
		RETURN_FALSE;

	Mat image3;
	if( image2.cols > image2.rows )
	{
		int dp = image2.cols - image2.rows;
		Mat(image2, Rect(dp/2, 0, image2.rows, image2.rows)).copyTo(image3);
	}
	else
	{
		int dp = image2.rows- image2.cols;
		Mat(image2, Rect(0, dp/2, image2.cols, image2.cols)).copyTo(image3);
	}

	Mat image;
	resize(image3, image, Size(400, 400));
	Mat gryImg1;
	Mat gryImg2;
	Mat gryImg3;
	Mat canImg;
	GaussianBlur(image, image, Size(5, 5), 0, 0);
	Canny(image, canImg, 1000, 1500, 5);
	cvtColor(image, gryImg1, CV_BGR2GRAY);
	cvtColor(image, gryImg2, CV_BGR2GRAY);
	threshold(gryImg1, gryImg1, 250, 255, CV_THRESH_BINARY);
	threshold(gryImg2, gryImg2, 64, 255, CV_THRESH_BINARY);

	Mat resultImg;
	resultImg.create(400, 400, CV_8UC3);
	resultImg.setTo(0);

	for(int i = 0; i < image.rows; i++)
	{
		for(int j = 0; j < image.cols; j++)
		{
			if( gryImg1.at<uchar>(i, j) > 128 )
				circle(resultImg, Point(j, i), 0, CV_RGB(0, 0, 0));
			else if( gryImg2.at<uchar>(i, j) > 128 )
				circle(resultImg, Point(j, i), 0, CV_RGB(255, 200, 50));
			else
				circle(resultImg, Point(j, i), 2, CV_RGB(100, 63, 15), -1);
		}
	}
	
	for(int i = 0; i < image.rows; i++)
	{
		for(int j = 0; j < image.cols; j++)
		{
			if( canImg.at<uchar>(i, j) > 128 )
				circle(resultImg, Point(j, i), 3, CV_RGB(63, 31, 15), -1);
		}
	}

	Mat bgImg = imread("bg.jpg");
	std::vector<Point2f> p0;
	std::vector<Point2f> p1;

	p0.push_back(Point2f(0, 0));
	p0.push_back(Point2f(400, 0));
	p0.push_back(Point2f(400, 400));
	p0.push_back(Point2f(0, 400));

	p1.push_back(Point2f(320, 233));
	p1.push_back(Point2f(544, 241));
	p1.push_back(Point2f(564, 513));
	p1.push_back(Point2f(266, 503));


	Mat perImg;
	Mat perMat = getPerspectiveTransform(p0, p1);
	Mat maskImg = imread("mask.jpg", 0);
	//std::cout << "perMat : " << perMat << std::endl;
	warpPerspective(resultImg, perImg, perMat, Size(bgImg.cols, bgImg.rows));
	threshold(perImg, gryImg1, 1, 255, CV_THRESH_BINARY);
	bgImg.copyTo(resultImg);
	perImg.copyTo(resultImg, gryImg1);
	threshold(maskImg, gryImg1, 250, 255, CV_THRESH_BINARY_INV);
	bgImg.copyTo(resultImg, gryImg1);
	//imshow("resultImg", resultImg);
	//imshow("gryImg1", gryImg1);
    imwrite( outfile, resultImg );

	//waitKey();


}


PHP_FUNCTION(to_pancake)
{
	php_pancake(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
