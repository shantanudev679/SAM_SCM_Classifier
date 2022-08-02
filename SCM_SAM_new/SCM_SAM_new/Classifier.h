#pragma once
#include <iostream>
class Classifier
{

public:
	float** meanVector;
	int*** ImageBuffer;
	static const int bands = 191;
	void getClasses(FILE* fp, int* numClass, unsigned int freq[]);
	void getMeanVector(FILE* fp, unsigned int freq[], int numClasses);
	void setImageBuffer(FILE* in);
	void Parallel_SCM(FILE* in, float** meanVector, int numClasses, int num);
	void Parallel_SAM(FILE* in, float** meanVector, int numClasses, int num);
	long double calcCorr(int imageVector[], float sampleVector[]);
	long double calculateAngle(float imageVector[], float sampleVector[]);

};
