// SAM_SCM.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Classifier.h"
#include "CreateImage.h"
#include <thread>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

int main()
{
	//path of files
	char sampleFilePath[] = "C:\\Users\\ASUS\\OneDrive\\Desktop\\Data\\DC_Roi_Samples.roi";
	char inputFilePath[] = "C:\\Users\\ASUS\\OneDrive\\Desktop\\Data\\dc__mall.asc";
	Classifier classifier;
	CreateImage image;

	FILE* fp = fopen(sampleFilePath, "r");

	int numClasses = 0;
	int NoOfClasses = 10;

	unsigned int* freq = new unsigned int[NoOfClasses];

	classifier.getClasses(fp, &numClasses, freq);

	cout << "Number Of Classes : " << numClasses << endl;
	cout << "Buffering image, please wait, dont give up :)" << endl;
	classifier.getMeanVector(fp, freq, numClasses);

	FILE* in = fopen(inputFilePath, "r");
	classifier.setImageBuffer(in);// storing image from file to an ImageBuffer 2D array
	int choice = 0;
	int threads = 0;
	int maxThreads = thread::hardware_concurrency();// maximum threads available

	/*for (int i = 1; i <= maxThreads; ++i) {
		classifier.Parallel_SAM(in, classifier.meanVector, numClasses, i);
	}*/

	do // for menu driven
	{
		cout << "Enter 1 for SAM and 2 for SCM and 3 for exit: ";
		cin >> choice;
		if (choice == 3) break;
		cout << "Maximum Threads available: " << maxThreads << endl;
		cout << "Enter choice of threads: ";
		cin >> threads;
		if (threads > maxThreads) threads = maxThreads;
		switch (choice)
		{
		case 1: //choice 1 for SAM
			classifier.Parallel_SAM(in, classifier.meanVector, numClasses, threads);
			image.createImageSAM();//image creation
			break;
		case 2: //choice 2 for SCM
			classifier.Parallel_SCM(in, classifier.meanVector, numClasses, threads);
			image.createImageSCM();//image creation
			break;
		default: cout << "Enter correct choice" << endl;
		}
	} while (choice != 3);

	fclose(fp);
	fclose(in);

	std::cout << "END!\n";
	return 0;
}

