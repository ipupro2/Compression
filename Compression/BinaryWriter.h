#pragma once
#include <cstdio>
#include <cstring>
class BinaryWriter
{
private:
	FILE* writer;
	char curByte;
	char bytePos;

	char* buffer;
	size_t bufferIndex;
	const size_t maxBufferSize = 4194304;

	void WriteToBuffer(char c);
public:
	bool IsOpened();
	char RemainBits();
	void MoveTo(int index);
	void FullByte();
	void WriteBit(char c);
	void WriteByte(char c);
	void WriteInt(int c);
	void WriteRemain();
	void WriteBuffer();

	BinaryWriter(const char* fileName);
	BinaryWriter(FILE* &file);
	~BinaryWriter();
};

