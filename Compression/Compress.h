#include "Huffman.h"
#include "BinaryWriter.h"
#include "BinaryReader.h"

struct Header
{
	long characterCount;
	string fileName;
};
void EncodeDescription(BinaryWriter& writer, const char* inFileName, vector<Node*>& datas);
void EncodeFileData(BinaryReader& reader, BinaryWriter& writer, vector<Node*>& datas);
Node* DecodeTree(BinaryReader& reader);
void Compress(const char* inFileName, BinaryWriter& writer, const char* directory = "/");
void CompressFile(const char* inFileName, const char* outFileName);
void Decompress(const char* fileName);