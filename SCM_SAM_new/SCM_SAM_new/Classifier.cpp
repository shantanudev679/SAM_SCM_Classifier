#include "Classifier.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <thread>
#include <vector>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

static const int bands = 191;//total bands of each  pixel
void SCM(int i, int j, Classifier* ptr, int*** ImageBuffer, int** OutputBuffer, float** meanVector, int numClasses, int WindowSizeHeight, int WindowSizeWidth);
void SAM(int i, int j, Classifier* ptr, int*** ImageBuffer, int** OutputBuffer, float** meanVector, int numClasses, int WindowSizeHeight, int WindowSizeWidth);

void Classifier::setImageBuffer(FILE* in) {
	int imageHeight = 1280, imageWidth = 307;
	ImageBuffer = new int** [imageHeight];//initializing the  image buffer
	for (int i = 0; i < imageHeight; i++)
	{
		ImageBuffer[i] = new int* [imageWidth];
		for (int j = 0; j < imageWidth; ++j)
		{
			ImageBuffer[i][j] = new int[bands];
			memset(ImageBuffer[i][j], 0, bands);
		}
	}

	int row, col, value; int r = 0, c = 0;
	while (!feof(in)) {//reading file and storing the  values in ImageBuffer
		for (int i = 0; i < imageHeight; ++i)
		{
			for (int j = 0; j < imageWidth; ++j)
			{
				fscanf(in, "%d %d", &row, &col);
				if (feof(in)) break;
				for (int k = 0; k < bands; ++k)
				{
					fscanf(in, "%d", &value);
					ImageBuffer[i][j][k] = value;
				}
			}
		}

	}

}

void Classifier::getClasses(FILE* fp, int* numClass, unsigned int freq[]) {
	int prevClass = -1;

	*numClass = 0;//for total classes
	int curFreq = 0;
	unsigned int Class = 0;
	int counter = 0;
	while (!feof(fp)) {
		++counter;
		unsigned int row, col;
		fscanf(fp, "%d %d", &row, &col);
		if (feof(fp))
			break;

		unsigned int temp;
		for (int i = 0; i < bands; i++)
		{
			fscanf(fp, "%d", &temp);

		}

		fscanf(fp, "%d", &Class);

		if (prevClass != Class)//if there is a change in class value
		{
			if (prevClass != -1)//for handling first or 0th case
			{
				freq[prevClass] = curFreq;//freq of the previous class
			}

			curFreq = 0;
			*numClass = *numClass + 1;

			prevClass = Class;
		}
		curFreq++;
	}

	freq[Class] = curFreq;
	cout << "Frequency" << endl;//printing the frequency of all the classes
	for (int i = 1; i < *numClass + 1; ++i) {
		cout << freq[i] << " ";
	}
	cout << endl;
	rewind(fp);//rewind file pointer to the begining

}

void Classifier::getMeanVector(FILE* fp, unsigned int freq[], int numClasses) {
	int curClass = 1;
	//initializing the size of the meanVector of sample data
	meanVector = (float**)malloc(numClasses * sizeof(float*));;
	for (int i = 0; i <= numClasses; ++i) {
		meanVector[i] = (float*)malloc(bands * sizeof(float));
	}
	for (int i = 0; i <= numClasses; ++i) {
		for (int j = 0; j < bands; ++j) {
			meanVector[i][j] = 0.0;
		}
	}

	while (!feof(fp)) {
		if (curClass == numClasses + 1) break;
		for (int i = 0; i < freq[curClass]; ++i) {

			int row, col; fscanf(fp, "%d %d", &row, &col);
			for (int j = 0; j < bands; ++j) {
				unsigned int val = 0;
				fscanf(fp, "%d", &val);
				meanVector[curClass][j] += val;//adding value for each band
			}
			int Class;
			fscanf(fp, "%d", &Class);//reading the class in the end
		}
		++curClass;
	}
	for (int i = 1; i <= numClasses; ++i) {
		for (int j = 0; j < bands; ++j) {
			meanVector[i][j] = meanVector[i][j] / freq[i];//divding with frequency of each class for calculating the average value
		}
	}

}

void Classifier::Parallel_SCM(FILE* in, float** meanVector, int numClasses, int num)
{
	vector<thread> threadVector;//vector for threads
	int imageHeight = 1280, imageWidth = 307;


	int** OutputBuffer = new int* [imageHeight];//image sized 2D OutpuBufferfor classification
	for (int i = 0; i < imageHeight; ++i) {
		OutputBuffer[i] = new int[imageWidth];
	}
	for (int i = 0; i < imageHeight; ++i) {
		for (int j = 0; j < imageWidth; ++j) {
			OutputBuffer[i][j] = 0;
		}
	}

	int numVectors = 0;
	int WindowSizeHeight, WindowSizeWidth;
	int numThreads = num;
	cout << "Launcing multiple threads : " << numThreads << endl;
	clock_t start, end;
	start = clock();//clock time starts
	WindowSizeHeight = imageHeight / numThreads;
	WindowSizeWidth = imageWidth / numThreads;
	int pixelRemainingHeight = 0;
	int pixelRemainingWidth = 0;

	// to Handle Remaining Pixels
	if (imageHeight > WindowSizeHeight * numThreads)
		pixelRemainingHeight = imageHeight - WindowSizeHeight * numThreads;
	if (imageWidth > WindowSizeWidth * numThreads)
		pixelRemainingWidth = imageWidth - WindowSizeWidth * numThreads;
	//to Handle Remaining Pixels

	//filling the thread vector
	for (int i = 0; i < imageHeight; )
	{
		for (int j = 0; j < imageWidth; )
		{
			//storing threads and splitting data accordingly
			threadVector.emplace_back(thread(SCM, i, j, this, ImageBuffer, OutputBuffer, meanVector, numClasses, WindowSizeHeight, WindowSizeWidth));
			++numVectors;
			j = j + WindowSizeWidth;
			if (imageWidth < j + WindowSizeWidth) break;//for handling remaining column pixels
		}
		i = i + WindowSizeHeight;
		if (imageHeight < i + WindowSizeHeight) break;//for handling remianing row pixels
	}
	int remRowStart = imageHeight - pixelRemainingHeight - 1;//start for remaining rows
	int remColStart = imageWidth - pixelRemainingWidth - 1;//start for remaining columns
	if (pixelRemainingWidth > 0) {
		++numVectors;
		threadVector.emplace_back(thread(SCM, 0, remColStart, this, ImageBuffer, OutputBuffer,
			meanVector, numClasses, imageHeight, pixelRemainingWidth));//to handle remaining columns
	}
	if (pixelRemainingHeight > 0) {
		++numVectors;
		threadVector.emplace_back(thread(SCM, remRowStart, 0, this, ImageBuffer, OutputBuffer,
			meanVector, numClasses, pixelRemainingHeight, imageWidth - pixelRemainingWidth));// to hanlde remaining rows
	}
	for (int i = 0; i < numVectors; ++i) {
		threadVector[i].join();//executing the threads
	}
	end = clock();//clock time ends
	double cpu_time_used;
	cpu_time_used = (end - start);
	cpu_time_used = cpu_time_used / CLOCKS_PER_SEC;//calculating time of execution of the all threads
	cout << "Time used : " << cpu_time_used << endl;
	cout << "Writing file" << endl;
	FILE* output = fopen("C:\\Users\\ASUS\\OneDrive\\Desktop\\Data\\SCM_out.txt", "w+");
	//storing the OutputBuffer in a file
	for (int i = 0; i < imageHeight; ++i) {
		for (int j = 0; j < imageWidth; ++j) {
			fprintf(output, "%d  ", OutputBuffer[i][j]);
		}
	}
	for (int i = 0; i < imageHeight; ++i) {
		delete OutputBuffer[i];
	}
	delete OutputBuffer;
	fclose(output);
}

void Classifier::Parallel_SAM(FILE* in, float** meanVector, int numClasses, int num)
{
	vector<thread> threadVector;//vector for threads
	int imageHeight = 1280, imageWidth = 307;


	int** OutputBuffer = new int* [imageHeight];//image sized 2D OutpuBufferfor classification
	for (int i = 0; i < imageHeight; ++i) {
		OutputBuffer[i] = new int[imageWidth];
	}
	for (int i = 0; i < imageHeight; ++i) {
		for (int j = 0; j < imageWidth; ++j) {
			OutputBuffer[i][j] = 0;
		}
	}

	int numVectors = 0;
	int WindowSizeHeight, WindowSizeWidth;
	int numThreads = num;
	cout << "Launcing multiple threads : " << numThreads << endl;
	clock_t start, end;
	start = clock();//clock time starts
	WindowSizeHeight = imageHeight / numThreads;
	WindowSizeWidth = imageWidth / numThreads;
	int pixelRemainingHeight = 0;
	int pixelRemainingWidth = 0;

	// to Handle Remaining Pixels
	if (imageHeight > WindowSizeHeight * numThreads)
		pixelRemainingHeight = imageHeight - WindowSizeHeight * numThreads;
	if (imageWidth > WindowSizeWidth * numThreads)
		pixelRemainingWidth = imageWidth - WindowSizeWidth * numThreads;
	//to Handle Remaining Pixels

	//filling the thread vector
	for (int i = 0; i < imageHeight; )
	{
		for (int j = 0; j < imageWidth; )
		{
			threadVector.emplace_back(thread(SAM, i, j, this, ImageBuffer, OutputBuffer, meanVector, numClasses, WindowSizeHeight, WindowSizeWidth));
			++numVectors;
			j = j + WindowSizeWidth;
			if (imageWidth < j + WindowSizeWidth) break;//for handling remaining column pixels
		}
		i = i + WindowSizeHeight;
		if (imageHeight < i + WindowSizeHeight) break;//for handling remaining row pixels
	}
	int remRowStart = imageHeight - pixelRemainingHeight - 1;//start for remaining rows
	int remColStart = imageWidth - pixelRemainingWidth - 1;//start for remaining columns
	if (pixelRemainingWidth > 0) {
		++numVectors;
		threadVector.emplace_back(thread(SAM, 0, remColStart, this, ImageBuffer, OutputBuffer,
			meanVector, numClasses, imageHeight, pixelRemainingWidth));//to handle remaining columns
	}
	if (pixelRemainingHeight > 0) {
		++numVectors;
		threadVector.emplace_back(thread(SAM, remRowStart, 0, this, ImageBuffer, OutputBuffer,
			meanVector, numClasses, pixelRemainingHeight, imageWidth - pixelRemainingWidth));// to hanlde remaining rows
	}
	for (int i = 0; i < numVectors; ++i) {
		threadVector[i].join();//executing the threads
	}
	end = clock();//clock time ends
	double cpu_time_used;
	cpu_time_used = (end - start);
	cpu_time_used = cpu_time_used / CLOCKS_PER_SEC;//calculating time of execution of the all threads
	cout << "Time used : " << cpu_time_used << endl;
	cout << "Writing file" << endl;
	FILE* output = fopen("C:\\Users\\ASUS\\OneDrive\\Desktop\\Data\\SAM_out.txt", "w+");
	//storing the OutputBuffer in a file
	for (int i = 0; i < imageHeight; ++i) {
		for (int j = 0; j < imageWidth; ++j) {
			fprintf(output, "%d  ", OutputBuffer[i][j]);
		}
	}
	for (int i = 0; i < imageHeight; ++i) {
		delete OutputBuffer[i];
	}
	delete OutputBuffer;
	fclose(output);
}

long double Classifier::calculateAngle(float imageVector[], float sampleVector[]) {
	long double dotProd = 0.0, modImage = 0.0, modSample = 0.0;

	for (int i = 0; i < bands; i++) {
		dotProd += imageVector[i] * sampleVector[i];//dot product  summation of image  and  sample values
		modImage += imageVector[i] * imageVector[i];//squared mod of imageVector
		modSample += sampleVector[i] * sampleVector[i];//squared mod of sampleVector
	}

	modImage = sqrt(modImage);//mod of image vector
	modSample = sqrt(modSample);//mod of sample vector
	long double den = modImage * modSample;//calculating the denominator

	return acos(dotProd / den);//returning the inverse of cosine
}

void SAM(int i, int j, Classifier* ptr, int*** ImageBuffer, int** OutputBuffer, float** meanVector, int numClasses, int WindowSizeHeight, int WindowSizeWidth) {
	float* imageVector = new float[bands];

	int row, col, value;
	long double angle = 100, curAngle = 100; int classToAssign = -1;
	for (int ii = i; ii < i + WindowSizeHeight; ii++)
	{
		for (int jj = j; jj < j + WindowSizeWidth; jj++)
		{

			for (int k = 0; k < bands; ++k)
			{
				imageVector[k] = ptr->ImageBuffer[ii][jj][k];//storing image row in an image vector
			}

			for (int Class = 1; Class <= numClasses; ++Class)
			{
				curAngle = ptr->calculateAngle(imageVector, ptr->meanVector[Class]);//calculating angle with all the classes
				if (curAngle < angle)
				{
					classToAssign = Class;//assigning class according to the minimum angle
					angle = curAngle;
				}
			}

			OutputBuffer[ii][jj] = classToAssign;//setting the outputbuffer with assigned class
			angle = 100; curAngle = 100;
			classToAssign = -1;
		}

	}

	delete imageVector;
}

long double Classifier::calcCorr(int imageVector[], float sampleVector[]) {
	long double sumSi = 0.0, sumSj = 0.0, sumSiSj = 0.0, sqSumSi = 0.0, sqSumSj = 0.0;

	for (int band = 0; band < bands; ++band) {
		sumSi += sampleVector[band];//summation Si
		sumSj += imageVector[band];//summation Sj
		sumSiSj += sampleVector[band] * imageVector[band];//summation of product of  Si Sj
		sqSumSi += sampleVector[band] * sampleVector[band];//summation of square of Si
		sqSumSj += imageVector[band] * imageVector[band];//summation of square of Sj
	}
	//dividing by bands and not multiplying to prevent overflow
	long double pOfSum = sumSi * sumSj;
	pOfSum = pOfSum / bands;
	long double numerator = sumSiSj - pOfSum;//numerator
	long double sqrt1 = sqrt(sqSumSi - (sumSi * sumSi) / bands);//denominator 1
	long double sqrt2 = sqrt(sqSumSj - (sumSj * sumSj) / bands);//denominator 2

	return (numerator / (sqrt1 * sqrt2));
}

void SCM(int i, int j, Classifier* ptr, int*** ImageBuffer, int** OutputBuffer, float** meanVector, int numClasses, int WindowSizeHeight, int WindowSizeWidth) {


	int* imageVector = new int[bands];

	int row, col, value;
	long double correlation = -100, curCorrelation; int classToAssign = -1; int e = 0;
	for (int ii = i; ii < i + WindowSizeHeight; ii++)
	{
		for (int jj = j; jj < j + WindowSizeWidth; jj++)
		{

			for (int k = 0; k < bands; ++k)
			{
				imageVector[k] = ptr->ImageBuffer[ii][jj][k];//storing image row in an image vector
			}

			for (int Class = 1; Class <= numClasses; ++Class)
			{
				curCorrelation = ptr->calcCorr(imageVector, ptr->meanVector[Class]);//calculating correlation with all the classes
				if (curCorrelation > correlation)
				{
					classToAssign = Class;//assigning class according to the maximum correlation
					correlation = curCorrelation;
				}
			}

			OutputBuffer[ii][jj] = classToAssign;//setting the outputbuffer with assigned class
			correlation = -100; curCorrelation = 0;
			classToAssign = -1;
		}

	}

	delete imageVector;
}
