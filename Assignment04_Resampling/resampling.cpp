#include "resampling.h"

void Resampler::antiAliasingFilter(const vector<vector<unsigned char>>& inputImage, vector<vector<unsigned char>>& outputImage, int width, int height)
{
    int kernelSize = 2 * ceil(3 * sigma) + 1; // Kernel size based on sigma
    int halfSize = kernelSize / 2;
    vector<vector<double>> kernel(kernelSize, vector<double>(kernelSize));

    // Create Gaussian kernel
    double sum = 0.0;

    for (int i = -halfSize; i < halfSize + 1; i++) {
        for (int j = -halfSize; j < halfSize + 1; j++) {
            kernel[i + halfSize][j + halfSize] = exp(-(i^2 + j^2) / (2 * sigma * sigma));
            sum += kernel[i + halfSize][j + halfSize];
        }
    }

    // Normalize the kernel
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] /= sum;
        }
    }

    // Apply filter
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double value = 0.0;

            for (int ki = -halfSize; ki < halfSize + 1; ki++) {
                for (int kj = -halfSize; kj < halfSize + 1; kj++) {
                    int x = min(max(i + ki, 0), height - 1);
                    int y = min(max(j + kj, 0), width - 1);
                    value += inputImage[x][y] * kernel[ki + halfSize][kj + halfSize];
                }
            }
            outputImage[i][j] = static_cast<unsigned char>(value);
        }
    }
}

void Resampler::downsampling(const vector<vector<unsigned char>>& inputImage, vector<vector<unsigned char>>& outputImage, int width, int height)
{
    int newWidth = width / 2;
    int newHeight = height / 2;
    outputImage.resize(newHeight, vector<unsigned char>(newWidth));

    for (int i = 0; i < newHeight; ++i) {
        for (int j = 0; j < newWidth; ++j)
            outputImage[i][j] = inputImage[i * 2][j * 2];
    }
}

void Resampler::upsampling(const vector<vector<unsigned char>>& inputImage, vector<vector<unsigned char>>& outputImage, int width, int height)
{
    int newWidth = width * 2;
    int newHeight = height * 2;
    outputImage.resize(newHeight, vector<unsigned char>(newWidth));

    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            outputImage[i][j] = inputImage[i / 2][j / 2];
        }
    }
}