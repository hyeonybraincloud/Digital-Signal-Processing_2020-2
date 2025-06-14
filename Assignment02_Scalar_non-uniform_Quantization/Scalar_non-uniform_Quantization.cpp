#include "Scalar_non-uniform_Quantization.h"

void Image::Set_Bit(int Bit)
{
	m_iBit = Bit;
}

void Image::Set_Ch(int Channel)
{
	m_iCh = Channel;
}

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

void Image::Initiate_Vector()
{
	m_ui16Comp.resize(m_iCh, vector<unsigned short>(HEIGHT * WIDTH, 0));
	m_ui8Comp.resize(m_iCh, vector<unsigned char>(HEIGHT * WIDTH * 2, 0));
}

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

void Image::Quantization(FILE* file)
{
	// Lloyd-Max Algorithm Initialization
	const int numLevels = 256;	// Number of quantization levels for 8-bit
	double minValue = 0.0;
	double maxValue = 1023.0;	// 10-bit MAX value
	double stepSize = (maxValue - minValue) / numLevels;

	vector<double> decision_Levels(numLevels + 1);
	vector<double> reconstruction_Levels(numLevels);

	// Initialize decision levels and reconstruction levels
	for (int i = 0; i < numLevels + 1; i++)
		decision_Levels[i] = minValue + i * stepSize;

	for (int i = 0; i < numLevels; i++)
		reconstruction_Levels[i] = (decision_Levels[i] + decision_Levels[i + 1]) / 2.0;

	// Interative Lloyd-Max Optimization
	for (int iteration = 0; iteration < 10; iteration++)
	{
		// Update reconstruction levels(centroids)
		for (int i = 0; i < numLevels; i++)
		{
			double sum = 0.0;
			int count = 0;

			for (int ch = 0; ch < m_iCh; ch++) {
				for (int j = 0; j < m_iSize[ch]; j++) {
					if (m_ui16Comp[ch][j] >= decision_Levels[i] && m_ui16Comp[ch][j] < decision_Levels[i + 1]) {
						sum += m_ui16Comp[ch][j];
						count++;
					}
				}
			}

			if (count > 0)
				reconstruction_Levels[i] = sum / count;
			else {
				if (i == 0)
					reconstruction_Levels[i] = reconstruction_Levels[i + 1];
				else if (i == numLevels - 1)
					reconstruction_Levels[i] = reconstruction_Levels[i - 1];
				else
					reconstruction_Levels[i] = (reconstruction_Levels[i - 1] + reconstruction_Levels[i + 1]) / 2.0;
			}
		}

		// Update decision Levels (midpoints)
		for (int i = 1; i < numLevels; i++)
			decision_Levels[i] = (reconstruction_Levels[i - 1] + reconstruction_Levels[i]) / 2.0;
	}

	// Quantization process
	for (int ch = 0; ch < m_iCh; ch++) {
		for (int i = 0; i < m_iSize[ch]; i++) {
			for (int level = 0; level < numLevels; level++) {
				if (m_ui16Comp[ch][i] >= decision_Levels[level] && m_ui16Comp[ch][i] < decision_Levels[level + 1]) {
					m_ui16Comp[ch][i] = static_cast<unsigned short>(reconstruction_Levels[level]);
					break;
				}
			}
		}

		// Write quantized data to file
		fwrite(&(m_ui16Comp[ch][0]), sizeof(unsigned short), m_iSize[ch], file);
	}
}

void Image::Reconstruction()
{
	// Reconstruct using stored reconstruction levels
	const int numLevels = 256; // Number of quantization levels for 8-bit
	double minValue = 0;
	double maxValue = 1023;
	double stepSize = (maxValue - minValue) / numLevels;

	vector<double> decision_Levels(numLevels + 1);
	vector<double> reconstruction_Levels(numLevels);

	// Initialize decision levels and reconstruction levels
	for (int i = 0; i <= numLevels; i++) {
		decision_Levels[i] = minValue + i * stepSize;
	}
	for (int i = 0; i < numLevels; i++) {
		reconstruction_Levels[i] = (decision_Levels[i] + decision_Levels[i + 1]) / 2.0;
	}

	// Reconstruct image
	for (int ch = 0; ch < m_iCh; ch++) {
		for (int i = 0; i < m_iSize[ch]; i++) {
			for (int level = 0; level < numLevels; level++) {
				if (m_ui16Comp[ch][i] >= decision_Levels[level] && m_ui16Comp[ch][i] < decision_Levels[level + 1]) {
					m_ui16Comp[ch][i] = static_cast<unsigned short>(reconstruction_Levels[level]);
					break;
				}
			}
		}
	}
}

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
