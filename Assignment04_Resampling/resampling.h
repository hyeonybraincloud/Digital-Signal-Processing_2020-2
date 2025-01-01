#ifndef __RESAMPLING_H
#define __RESAMPLING_H

#define _CRT_SECURE_NO_WARNINGS

#define WIDTH 832
#define HEIGHT 480

#include <iostream>
#include <vector>

using namespace std;

class Resampler {
private:
	double sigma;

public:
	Resampler(double sigma) : sigma(sigma) {}

	void antiAliasingFilter(const vector<vector<unsigned char>>& inputImage, vector<vector<unsigned char>>& outputImage, int width, int height);
	void downsampling(const vector<vector<unsigned char>>& inputImage, vector<vector<unsigned char>>& outputImage, int width, int height);
	void upsampling(const vector<vector<unsigned char>>& inputImage, vector<vector<unsigned char>>& outputImage, int width, int height);
};
#endif