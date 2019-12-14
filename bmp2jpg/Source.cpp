#include <string>
#include <iostream>
#include "jpge.h"
#include "stb_image.c"

int main()
{
	char pSrc_filename[256];
	printf("Nhap ten file anh (duoi bmp): ");
	scanf("%s", &pSrc_filename);
	char pDst_filename[256];
	printf("Nhap ten file anh sau khi nen (duoi jpg): ");
	scanf("%s", &pDst_filename);

	int quality_factor = 90;
	//bool optimize_huffman_tables = false;

	// Load the source image.
	const int req_comps = 3; // request RGB image
	int width = 0, height = 0, actual_comps = 0;
	uint8 *pImage_data = stbi_load(pSrc_filename, &width, &height, &actual_comps, req_comps);
	
	if (!pImage_data)
	{
		printf("Failed loading file \"%s\"!\n", pSrc_filename);
		return EXIT_FAILURE;
	}

	// Fill in the compression parameter structure.
	jpge::params params;
	params.m_quality = quality_factor;
	params.m_subsampling = jpge::H2V2;
	//params.m_two_pass_flag = optimize_huffman_tables;


	if (!jpge::compress_image_to_jpeg_file(pDst_filename, width, height, req_comps, pImage_data, params))
		printf("Fail.\n");
	else printf("Success.\n");
	//system("pause");
}