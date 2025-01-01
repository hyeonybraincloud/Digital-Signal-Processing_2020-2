#ifndef __SCALAR_NON_UNI_QUANT_H
#define __SCALAR_NON_UNI_QUANT_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cmath>

static const int WIDTH = 960;
static const int HEIGHT = 540;

using namespace std;

class Image
{
private:
	int m_iBit;
	int m_iCh;
	int m_iSize[3];
	vector <vector<unsigned short>> m_ui16Comp;
	vector <vector<unsigned char>> m_ui8Comp;

public:
	void Set_Bit(int Bit);
	void Set_Ch(int Channel);
	void Set_Size(int height, int width);
	void Initiate_Vector();
	void readOneFrame(FILE* file);
	void Quantization(FILE* file);
	void Reconstruction();
	void PSNR();
};

#endif