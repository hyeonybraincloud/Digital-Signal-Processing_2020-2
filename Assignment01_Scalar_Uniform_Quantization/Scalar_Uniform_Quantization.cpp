#include "Scalar_Uniform_Quantization.h"

// read One Frame
void Image::readOneFrame(FILE* file)
{
	int bitFactor = (m_iBit <= 8) ? 1 : 2;

	for (int ch = 0; ch < m_iCh; ch++)
	{
		fread(&(m_ui8Comp[ch][0]), sizeof(unsigned char), m_iSize[ch] * bitFactor, file);

		if (m_iBit == 10)
		{
			for (int i = 0; i < m_iSize[ch]; i++)
				m_ui16Comp[ch][i] = (m_ui8Comp[ch][i * 2] + (m_ui8Comp[ch][i * 2 + 1] << 8));
		}
	}
}

// Set the bit-depth
void Image::Set_Bit(int Bit)
{
	m_iBit = Bit;
}

// Set the number of the channel
void Image::Set_Ch(int Channel)
{
	m_iCh = Channel;
}

// Set the number of the value of the Channel
void Image::Set_Size(int height, int width)
{
	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
			m_iSize[i] = height * width;
		else
			m_iSize[i] = (height / 2) * (width / 2);
	}
}

// Quantization and make out a 8-bit image
void Image::Quantization(FILE* file)
{
	for (int ch = 0; ch < m_iCh; ch++)
	{
		for (int i = 0; i < m_iSize[ch]; i++)
			m_ui16Comp[ch][i] >>= 2;

		fwrite(&(m_ui16Comp[ch][0]), sizeof(unsigned short), m_iSize[ch], file);
	}
}

// Initiate vectors.
void Image::Initiate_Vector()
{
	m_ui16Comp.resize(m_iCh, vector<unsigned short>(540 * 960, 0));
	m_ui8Comp.resize(m_iCh, vector<unsigned char>(540 * 960 * 2, 0));
}

// Reconstruction; 8-bit -> 10-bit
void Image::Reconstruction()
{
	for (int ch = 0; ch < m_iCh; ch++)
	{
		for (int i = 0; i < m_iSize[ch]; i++)
			m_ui8Comp[ch][i] <<= 2;
	}
}

// measure PSNR
void Image::PSNR()
{
	double sum;
	double MSE;
	double MAX = 1023.0;
	double PSNR;
	for (int ch = 0; ch < m_iCh; ch++)
	{
		sum = 0.0;
		MSE = 0.0;
		PSNR = 0.0;

		for (int i = 0; i < m_iSize[ch]; i++)
			sum += pow((m_ui16Comp[ch][i] - m_ui8Comp[ch][i]), 2);
		MSE = sum / m_iSize[ch];
		PSNR = 10.0 * log10(pow(MAX, 2) / MSE);

		if (ch == 0)
			cout << "PSNR of  Y : " << PSNR << endl;			// PSNR of Y
		else if (ch == 1)
			cout << "PSNR of Cb : " << PSNR << endl;			// PSNR of Cb
		else
			cout << "PSNR of Cr : " << PSNR << endl;			// PSNR of Cr
	}
}