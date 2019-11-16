#pragma once
#include <cstdio>

class BinaryReader
{
private:
	FILE* reader;
	char curByte;
	int bitPos;

public:
	bool IsOpened();
	bool IsEOF();
	int FileLength();
	void EmptyByte();
	char ReadBit();
	char ReadByte();
	int ReadInt();
	void CompleteByte();

	BinaryReader(const char* fileName);
	BinaryReader(FILE* file);
	~BinaryReader();
};

