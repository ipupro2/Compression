#include "Compress.h"
#include <cstring>

void EncodeDescription(FILE* &outFile, const char* inFileName, vector<Node*>& datas)
{
	//Nhúng header vào file - nếu nó đúng header thì mới được phép giải nén
	fwrite("tzip", 4, 1, outFile);

	int nDatas = datas.size();
	//Nhúng tên file ban đầu
	fwrite(inFileName, strlen(inFileName) + 1, 1, outFile);
	//Nhúng số lượng ký tự gốc vào file
	fwrite(&nDatas, sizeof(int), 1, outFile);

	//Nhúng dữ liệu và tần suất vào file
	for (int i = 0; i < nDatas; i++)
	{
		fwrite(&datas[i]->data, 1, 1, outFile);
		fwrite(&datas[i]->freq, sizeof(int), 1, outFile);
	}
	cout << "\nUsed " << (nDatas * (1 + sizeof(int)) + strlen(inFileName) + sizeof(int) + 4) << " bytes to store file descriptions\n";
}

void EncodeFileData(FILE*& inFile, FILE* &outFile, vector<Node*>& datas)
{
	Node* node = BuildHuffmanTree(datas);
	CodeBook dict;
	BuildCodeBook(node, dict);
	//Sắp xếp trước code book để truy xuất bằng tìm kiếm nhị phân nhanh hơn
	SortCodeBook(dict, 0, dict.size() - 1);

	fseek(inFile, 0, SEEK_SET);
	
	char bit = 0;
	string code;
	int pos = 7;
	char* s = new char[node->freq + 1];
	char* compressed = new char[node->freq + 1];
	int compressedIndex = 0;

	//Nhúng số ký tự ban đầu trong file
	fwrite(&node->freq, sizeof(int), 1, outFile);
	fread(s, node->freq, 1, inFile);

	for (int i = 0; i < node->freq; i++)
	{
		code = LookUpCodeBook(dict, s[i], 0, dict.size() - 1);
		for (int j = 0; j < code.size(); j++)
		{
			if (code[j] == '1')
				bit |= (1 << pos);
			pos--;
			if (pos == -1)
			{
				compressed[compressedIndex++] = bit;
				bit = 0;
				pos = 7;
			}
		}
	}
	compressed[compressedIndex++] = bit;
	compressed[compressedIndex] = '\0';
	fwrite(compressed, compressedIndex, 1, outFile);
}

void Compress(const char* inFileName, const char* outFileName)
{
	FILE* inFile;
	fopen_s(&inFile, inFileName, "rb");
	FILE* outFile;
	fopen_s(&outFile, outFileName, "wb");
	if (!inFile || !outFile)
	{
		if (inFile)
			fclose(inFile);
		if (outFile)
			fclose(outFile);
		cout << "File's not exist\n";
		return;
	}
	vector<Node*> datas = CountFrequency(inFile);
	EncodeDescription(outFile, inFileName, datas);
	EncodeFileData(inFile, outFile, datas);

	fclose(inFile);
	fclose(outFile);
}

void Decompress(const char* fileName)
{
	FILE* inFile;
	fopen_s(&inFile, fileName, "rb");
	if (!inFile)
		return;
	//Kiểm tra đây có phải header của file nén
	char* header = new char[4];
	fread(header, 4, 1, inFile);
	for (int i = 0; i < 4; i++)
	{
		if (header[i] != "tzip"[i])
		{
			cout << "Wrong format!!";
			return;
		}
	}
	delete[]header;

	char* path = new char[100];
	for (int i = 0; i < 100; i++)
	{
		path[i] = getc(inFile);
		if (path[i] == '\0')
			break;
	}

	FILE* outFile;
	fopen_s(&outFile, path, "wb");

	//Load lại dữ liệu và tần suất của chúng
	int nD;
	fread(&nD, sizeof(int), 1, inFile);
	vector<Node*>datas(nD);
	char data;
	int freq;
	for (int i = 0; i < nD; i++)
	{
		fread(&data, 1, 1, inFile);
		fread(&freq, sizeof(int), 1, inFile);
		datas[i] = CreateNode(data, freq);
	}
	//Xây lại cây huffman
	Node* node = BuildHuffmanTree(datas);

	if (!outFile)
		return;
	string bits;
	char c = 0, temp = 0;
	//Đọc số lượng ký tự có trong file gốc
	int length = 0;
	fread(&length, sizeof(int), 1, inFile);
	c = fgetc(inFile);
	while (!feof(inFile))
	{
		for (int i = 7; i >= 0; i--)
		{
			bits += ((char)((c >> i) & 1) + '0');
			if (Traverse(node, bits, temp))
			{
				fwrite(&temp, 1, 1, outFile);
				bits = "";
				length--;
				if (length == 0)
					break;
			}
		}
		if (length == 0)
			break;
		c = fgetc(inFile);
	}
	if (length > 0)
	{
		cout << "File is broken!\n";
	}
	_fcloseall();
}