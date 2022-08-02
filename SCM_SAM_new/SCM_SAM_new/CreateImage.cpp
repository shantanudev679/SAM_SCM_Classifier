#include "CreateImage.h"
#include <iostream>
#include <string>

#define _CRT_SECURE_NO_WARNINGS
using namespace std;

void CreateImage::createImageSCM()
{
	FILE* fp = fopen("C:\\Users\\ASUS\\OneDrive\\Desktop\\Data\\SCM_out.txt", "r");
	FILE* fpout = fopen("C:\\Users\\ASUS\\OneDrive\\Desktop\\Data\\Classified_SCM.raw", "wb+");
	int tempClass = 0;
	int ColourRed = 255, ColourGreen = 255, ColourBlue = 255;//red green blue values for color
	while (!feof(fp))
	{
		fscanf(fp, "%d", &tempClass);
		switch (tempClass)
		{
		case 1://giving 3 RGB values to the file position to create image
			ColourRed = 237;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 28;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 36;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		case 2:
			ColourRed = 34;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 177;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 76;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		case 3:
			ColourRed = 255;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 242;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 0;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		case 4:
			ColourRed = 0;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 0;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 0;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		case 5:
			ColourRed = 149;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 238;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 238;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		default:
			ColourRed = 255;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 255;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 255;
			fwrite(&ColourBlue, 1, 1, fpout);
		}

	}
	fclose(fp);
	fclose(fpout);
}

void CreateImage::createImageSAM()
{
	FILE* fp = fopen("C:\\Users\\ASUS\\OneDrive\\Desktop\\Data\\SAM_out.txt", "r");
	FILE* fpout = fopen("C:\\Users\\ASUS\\OneDrive\\Desktop\\Data\\Classified_SAM.raw", "wb+");
	int tempClass = 0;
	int ColourRed = 255, ColourGreen = 255, ColourBlue = 255;//red green blue values for color
	while (!feof(fp))
	{
		fscanf(fp, "%d", &tempClass);
		switch (tempClass)
		{
		case 1://giving 3 RGB values to the file position to create image
			ColourRed = 237;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 28;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 36;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		case 2:
			ColourRed = 34;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 177;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 76;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		case 3:
			ColourRed = 255;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 242;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 0;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		case 4:
			ColourRed = 0;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 0;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 0;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		case 5:
			ColourRed = 149;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 238;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 238;
			fwrite(&ColourBlue, 1, 1, fpout);
			break;
		default:
			ColourRed = 255;
			fwrite(&ColourRed, 1, 1, fpout);
			ColourGreen = 255;
			fwrite(&ColourGreen, 1, 1, fpout);
			ColourBlue = 255;
			fwrite(&ColourBlue, 1, 1, fpout);
		}

	}
	fclose(fp);
	fclose(fpout);
}
