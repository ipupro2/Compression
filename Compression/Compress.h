#include "Huffman.h"

struct Header
{
	long characterCount;
	string fileName;
};
void Compress(const char* inFileName, const char* outFileName);
void Decompress(const char* fileName);