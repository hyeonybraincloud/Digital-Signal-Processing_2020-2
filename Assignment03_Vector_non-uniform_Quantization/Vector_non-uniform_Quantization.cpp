#include "Vector_non-uniform_Quantization.h"

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
    // K-means clustering initialization for vector quantization
    const int numClusters = 256; // Number of quantization clusters
    const int vectorSize = 4;    // Size of the vector for grouping (e.g., 2x2 pixels)

    vector<vector<double>> centroids(numClusters, vector<double>(vectorSize, 0));
    vector<int> clusterAssignments(m_iSize[0] / vectorSize, -1);

    // Randomly initialize centroids
    std::srand(std::time(0));
    for (int i = 0; i < numClusters; i++) {
        for (int j = 0; j < vectorSize; j++) {
            centroids[i][j] = std::rand() % 1024; // Assuming 10-bit input
        }
    }

    bool centroidsUpdated = true;
    int maxIterations = 10;
    int iteration = 0;

    while (centroidsUpdated && iteration < maxIterations) {
        centroidsUpdated = false;
        iteration++;

        // Assign each vector to the closest centroid
        for (int i = 0; i < m_iSize[0] / vectorSize; i++) {
            vector<double> currentVector(vectorSize, 0);
            for (int j = 0; j < vectorSize; j++) {
                currentVector[j] = m_ui16Comp[0][i * vectorSize + j];
            }

            int bestCluster = -1;
            double bestDistance = numeric_limits<double>::max();

            for (int c = 0; c < numClusters; c++) {
                double distance = 0;
                for (int j = 0; j < vectorSize; j++) {
                    distance += std::pow(currentVector[j] - centroids[c][j], 2);
                }

                if (distance < bestDistance) {
                    bestDistance = distance;
                    bestCluster = c;
                }
            }

            if (clusterAssignments[i] != bestCluster) {
                centroidsUpdated = true;
                clusterAssignments[i] = bestCluster;
            }
        }

        // Update centroids
        vector<vector<double>> newCentroids(numClusters, vector<double>(vectorSize, 0));
        vector<int> clusterCounts(numClusters, 0);

        for (int i = 0; i < m_iSize[0] / vectorSize; i++) {
            int cluster = clusterAssignments[i];
            for (int j = 0; j < vectorSize; j++) {
                newCentroids[cluster][j] += m_ui16Comp[0][i * vectorSize + j];
            }
            clusterCounts[cluster]++;
        }

        for (int c = 0; c < numClusters; c++) {
            if (clusterCounts[c] > 0) {
                for (int j = 0; j < vectorSize; j++) {
                    centroids[c][j] = newCentroids[c][j] / clusterCounts[c];
                }
            }
        }
    }

    // Apply quantization
    for (int i = 0; i < m_iSize[0] / vectorSize; i++) {
        int cluster = clusterAssignments[i];
        for (int j = 0; j < vectorSize; j++) {
            m_ui16Comp[0][i * vectorSize + j] = static_cast<unsigned short>(centroids[cluster][j]);
        }
    }

    // Write quantized data to file
    fwrite(&(m_ui16Comp[0][0]), sizeof(unsigned short), m_iSize[0], file);
}

void Image::Reconstruction()
{
    // K-means reconstruction
    const int numClusters = 256;
    const int vectorSize = 4;
    vector<vector<double>> centroids(numClusters, vector<double>(vectorSize, 0));

    // Reconstruct using stored centroids and assignments
    for (int i = 0; i < m_iSize[0] / vectorSize; i++) {
        int cluster = m_ui16Comp[0][i * vectorSize] / 256; // Approximate cluster ID
        for (int j = 0; j < vectorSize; j++) {
            m_ui16Comp[0][i * vectorSize + j] = static_cast<unsigned short>(centroids[cluster][j]);
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