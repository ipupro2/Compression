#pragma once
#define _CRT_SECURE_NO_WARNINGS
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
	const size_t maxBufferSize = 4194304;//33554432

	void WriteToBuffer(const char& c);
public:
	bool IsOpened();
	char RemainBits();
	void FullByte();
	void WriteBit(const char& c);
	void WriteByte(const char& c);
	void WriteInt(const int& c);
	void WriteRemain();
	void WriteBuffer();

	BinaryWriter(const char* fileName);
	BinaryWriter(FILE* &file);
	~BinaryWriter();
};

