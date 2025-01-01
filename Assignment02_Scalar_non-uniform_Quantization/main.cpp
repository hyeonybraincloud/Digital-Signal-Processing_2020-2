#include "Scalar_non-Uniform_Quantization.h"

int main()
{
	Image* img = new Image;

	FILE* pFile = fopen("./input/RitualDance_960x540_10bit_420_frame100.yuv", "rb");
	// FILE* pFile = fopen("./input/RitualDance_960x540_10bit_420_frame200.yuv", "rb");
	// FILE* pFile = fopen("./input/RitualDance_960x540_10bit_420_frame250.yuv", "rb");
	// FILE* pFile = fopen("./input/RitualDance_960x540_10bit_420_frame300.yuv", "rb");
	// FILE* pFile = fopen("./input/RitualDance_960x540_10bit_420_frame350.yuv", "rb");

	img->Set_Bit(10);
	img->Set_Ch(3);
	img->Set_Size(HEIGHT, WIDTH);
	img->Initiate_Vector();

	// read the input data (10 bit)
	img->readOneFrame(pFile);
	fclose(pFile);

	// Quantization and make a 8-bit image
	pFile = fopen("./output/RitualDance_960x540_8bit_420_frame100.yuv", "wb");
	// pFile = fopen("./output/RitualDance_960x540_8bit_420_frame200.yuv", "wb");
	// pFile = fopen("./output/RitualDance_960x540_8bit_420_frame250.yuv", "wb");
	// pFile = fopen("./output/RitualDance_960x540_8bit_420_frame300.yuv", "wb");
	// pFile = fopen("./output/RitualDance_960x540_8bit_420_frame350.yuv", "wb");
	img->Quantization(pFile);
	fclose(pFile);
	img->Set_Bit(8);

	// To compare the original and the reconstruction, first of all, the original must be gained.
	img->Set_Bit(10);
	pFile = fopen("./input/RitualDance_960x540_10bit_420_frame100.yuv", "rb");
	// pFile = fopen("./input/RitualDance_960x540_10bit_420_frame200.yuv", "rb");
	// pFile = fopen("./input/RitualDance_960x540_10bit_420_frame250.yuv", "rb");
	// pFile = fopen("./input/RitualDance_960x540_10bit_420_frame300.yuv", "rb");
	// pFile = fopen("./input/RitualDance_960x540_10bit_420_frame350.yuv", "rb");
	img->readOneFrame(pFile);
	fclose(pFile);

	// Then, the reconstruction is performed and the result is got.
	img->Set_Bit(8);
	pFile = fopen("./output/RitualDance_960x540_8bit_420_frame100.yuv", "rb");
	// pFile = fopen("./output/RitualDance_960x540_8bit_420_frame200.yuv", "rb");
	// pFile = fopen("./output/RitualDance_960x540_8bit_420_frame250.yuv", "rb");
	// pFile = fopen("./output/RitualDance_960x540_8bit_420_frame300.yuv", "rb");
	// pFile = fopen("./output/RitualDance_960x540_8bit_420_frame350.yuv", "rb");
	img->readOneFrame(pFile);
	img->Reconstruction();

	// Measure PSNR
	img->PSNR();

	delete img;

	return 0;
}