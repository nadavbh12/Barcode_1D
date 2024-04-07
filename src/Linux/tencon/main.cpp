#include <string>
#include <stdlib.h>
#include <iostream>

#include "gallo/gallo.h"
#include "soros/soros.h"
#include "yun/yun.h"

// key
const char* keys = 
    "{h        help |                      | print help message                            }"
    "{file          | /file/dir/file_name  | test image file(.bmp .jpg .png)               }";

int main(int argc, char* argv[])
{
	cv::CommandLineParser cmd(argc, argv, keys);
    if (cmd.has("help") || !cmd.check())
    {
		cmd.printMessage();
		cmd.printErrors();
		return 0;
    }
	
	std::string fn = cmd.get<std::string>("file");
	
	iy::Gallo mGallo;
	iy::Soros mSoros;
	iy::Yun mYun;

	cv::Mat frame_gray;
	cv::Mat frame = cv::imread(fn.c_str());
	
	if(frame.data == NULL) {
		std::cerr << "error! read image" << std::endl;
		return -1;
	}

	cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

	cv::Rect g_rt = mGallo.process(frame_gray, 20);
        std::cout << "Gallo detection: x=" << g_rt.x << ", y=" << g_rt.y << ", width=" << g_rt.width << ", height=" << g_rt.height << std::endl;
	
	cv::Rect s_rt = mSoros.process(frame_gray, 20);
	std::cout << "Soros detection: x=" << s_rt.x << ", y=" << s_rt.y << ", width=" << s_rt.width << ", height=" << s_rt.height << std::endl;

	std::vector<iy::YunCandidate> list_barcode = mYun.process(frame_gray);
	if (!list_barcode.empty())
	{
		for (std::vector<iy::YunCandidate>::iterator it = list_barcode.begin(); it != list_barcode.end(); ++it)
		{
			if (it->isBarcode)
			{
				cv::Rect y_rt = it->roi;
				std::cout << "Yun barcode detection: x=" << y_rt.x << ", y=" << y_rt.y << ", width=" << y_rt.width << ", height=" << y_rt.height << std::endl;
			}
		}

		list_barcode.clear();
	}

        // Commenting out imshow and waitKey since we don't need to display the image anymore.
        //cv::imshow("frame", frame);
        //cv::waitKey(0);

	//cv::destroyAllWindows();

	return 0;
}
