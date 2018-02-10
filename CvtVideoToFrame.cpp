// Vedio2Img.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MySVM : public CvSVM
{
public:
	//获得svm的决策函数中的alpha数组  
	double * get_alpha_vector()
	{
		return this->decision_func->alpha;
	}

	//获得svm的决策函数中的rho参数,即偏移量  
	float get_rho()
	{
		return this->decision_func->rho;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 4)
	{
		cout << "input arguments error" << endl;
		return 0;
	}

	//遍历目录下视频文件
	string inputDir = argv[1];
	string outputDir = argv[2];
	
	//设定视频参数
	int interval = atoi(argv[3]);//每interval取一帧

	WIN32_FIND_DATA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[MAX_PATH] = { 0 };
	// 构造路径
	char FullPathName[MAX_PATH] = { 0 };
	strcpy(FilePathName, argv[1]);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFile(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		printf("搜索失败!\n");
		return 0;
	}

	//设定图片保存参数
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);    //png格式下，默认的参数为3.

	//提取视频中的图片

	int videoCount = 1;
	do
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		sprintf_s(FullPathName, "%s\\%s", argv[1], FindData.cFileName);

		VideoCapture cap;
		cap.open(FullPathName);
		Mat img;
		if (cap.isOpened())
		{
			cout << "start to extract video " << videoCount++ << "...\r";
			char directoryName[MAX_PATH] = { 0 };
			sprintf_s(directoryName, "%s\\%s", argv[2], FindData.cFileName);
			CreateDirectory(directoryName, NULL);

			int cnt = 0;
			int frameNum = 0;
			while (true)
			{
				cap >> img;
				if (!img.empty())
				{
					if (cnt++ == 0)
					{
						resize(img, img, Size(), 1, 1);
						char fileName[MAX_PATH] = { 0 };
						sprintf_s(fileName, "%s\\%s\\%06d.jpg", argv[2], FindData.cFileName, frameNum++);
						imwrite(fileName, img);
					}

					if (cnt >= interval)
					{
						cnt = 0;
					}

					//imshow("test",img);
					//waitKey(10);
				}
				else
				{
					break;
				}

			}

		}
	} while (::FindNextFile(hError, &FindData));


	getchar();
	return 0;
}

