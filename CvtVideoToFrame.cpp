// Vedio2Img.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MySVM : public CvSVM
{
public:
	//���svm�ľ��ߺ����е�alpha����  
	double * get_alpha_vector()
	{
		return this->decision_func->alpha;
	}

	//���svm�ľ��ߺ����е�rho����,��ƫ����  
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

	//����Ŀ¼����Ƶ�ļ�
	string inputDir = argv[1];
	string outputDir = argv[2];
	
	//�趨��Ƶ����
	int interval = atoi(argv[3]);//ÿintervalȡһ֡

	WIN32_FIND_DATA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[MAX_PATH] = { 0 };
	// ����·��
	char FullPathName[MAX_PATH] = { 0 };
	strcpy(FilePathName, argv[1]);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFile(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		printf("����ʧ��!\n");
		return 0;
	}

	//�趨ͼƬ�������
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);    //png��ʽ�£�Ĭ�ϵĲ���Ϊ3.

	//��ȡ��Ƶ�е�ͼƬ

	int videoCount = 1;
	do
	{
		// ����.��..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// ��������·��
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

