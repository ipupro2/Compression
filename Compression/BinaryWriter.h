#pragma once
#include <cstdio>
#include <cstring>
class BinaryWriter
{
private:
	FILE* writer;
	char curByte;
	char bytePos;
public:
	bool IsOpened();
	char RemainBits();
	void MoveTo(int index);
	void FullByte();
	void WriteBit(char c);
	void WriteByte(char c);
	void WriteInt(int c);
	void WriteRemain();

	BinaryWriter(const char* fileName);
	BinaryWriter(FILE* &file);
	~BinaryWriter();
};

