#include "Function.h"

void Swap(Node*& a, Node*& b)
{
	Node* temp = a;
	a = b;
	b = temp;
}

bool IsIn(vector<Node*>& datas, char c)
{
	for (int i = 0; i < datas.size(); i++)
	{
		if (c == datas[i]->data)
		{
			datas[i]->freq++;
			return true;
		}
	}
	Node* node = new Node({ c,1, NULL, NULL });
	datas.push_back(node);
	return false;
}

//vector<Node*> CountFrequency(FILE* &inFile)
//{
//	vector<Node*> datas;
//
//	fseek(inFile, 0, SEEK_END);
//	int fileLength = ftell(inFile);
//	fseek(inFile, 0, SEEK_SET);
//
//	char* s = new char[fileLength + 1];
//
//	fread(s, fileLength, 1, inFile);
//
//	for (int i = 0; i < fileLength; i++)
//		IsIn(datas, s[i]);
//	delete[] s;
//	return datas;
//}

vector<Node*> CountFrequency(FILE*& inFile)
{
	vector<Node*> datas;
	fseek(inFile, 0, SEEK_SET);
	unsigned char c = 0;
	vector<int> freqs(256);
	fread(&c, 1, 1, inFile);
	while (!feof(inFile))
	{
		freqs[(int)c]++;
		fread(&c, 1, 1, inFile);
	}
	for (int i = 0; i < 256; i++)
	{
		if (freqs[i] != 0)
		{
			Node* node = new Node({ (char)i, (unsigned int)freqs[i],NULL,NULL });
			datas.push_back(node);
		}
	}
 	return datas;
}