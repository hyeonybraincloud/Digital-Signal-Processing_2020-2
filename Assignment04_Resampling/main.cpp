#include "resampling.h"

int main()
{
	// 832X480 resolution image file
	FILE* pFile = fopen("./input/BasketballDrill_832x480_yuv420_8bit_frame0.yuv", "rb");
	// FILE* pFile = fopen("./input/BasketballDrill_832x480_yuv420_8bit_frame360.yuv", "rb");
	// FILE* pFile = fopen("./input/BQMall_832x480_yuv420_8bit_frame0.yuv", "rb");
	// FILE* pFile = fopen("./input/BQMall_832x480_yuv420_8bit_frame120.yuv", "rb");
	// FILE* pFile = fopen("./input/PartyScene_832x480_yuv420_8bit_frame0.yuv", "rb");
	// FILE* pFile = fopen("./input/PartyScene_832x480_yuv420_8bit_frame120.yuv", "rb");
	// FILE* pFile = fopen("./input/RaceHorsesC_832x480_yuv420_8bit_frame0.yuv", "rb");
	// FILE* pFile = fopen("./input/RaceHorsesC_832x480_yuv420_8bit_frame120.yuv", "rb");

	// 1920X1080 resolution image file
	// FILE* pFile = fopen("./input/BQTerrace_1920x1080_yuv420_8bit_frame0.yuv", "rb");
	// FILE* pFile = fopen("./input/Cactus_1920x1080_yuv420_8bit_frame200.yuv", "rb");
	// FILE* pFile = fopen("./input/Kimono1_1920x1080_yuv420_8bit_frame0.yuv", "rb");
	// FILE* pFile = fopen("./input/ParkScene_1920x1080_yuv420_8bit_frame0.yuv", "rb");
	// FILE* pFile = fopen("./input/ParkScene_1920x1080_yuv420_8bit_frame200.yuv", "rb");

	if (pFile == NULL)
	{
		cout << "It is denied to open the file." << endl;
		return 0;
	}

    // Allocate memory for input image
    vector<vector<unsigned char>> inputImage(HEIGHT, vector<unsigned char>(WIDTH));
    vector<vector<unsigned char>> filteredImage(HEIGHT, vector<unsigned char>(WIDTH));
    vector<vector<unsigned char>> downsampledImage;
    vector<vector<unsigned char>> upsampledImage;

    // Read image data
    for (int i = 0; i < HEIGHT; ++i) {
        fread(inputImage[i].data(), sizeof(unsigned char), WIDTH, pFile);
    }
    fclose(pFile);

    // Create a Resampler object
    double sigma = 1.0; // Standard deviation for Gaussian filter
    Resampler resampler(sigma);

    // Apply anti-aliasing filter
    resampler.antiAliasingFilter(inputImage, filteredImage, WIDTH, HEIGHT);

    // Downsampling
    resampler.downsampling(filteredImage, downsampledImage, WIDTH, HEIGHT);

    // Upsampling
    resampler.upsampling(downsampledImage, upsampledImage, WIDTH / 2, HEIGHT / 2);

    // Output results (example: save to file)
    FILE* outFile = fopen("./output/upsampled_image.yuv", "wb");

    if (outFile != NULL) {
        for (const auto& row : upsampledImage)
            fwrite(row.data(), sizeof(unsigned char), row.size(), outFile);

        fclose(outFile);
        cout << "Upsampled image written to ./output/upsampled_image.yuv" << endl;
    }
    else
        cout << "Failed to write the output file." << endl;

	return 0;
}