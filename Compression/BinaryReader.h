#pragma once
#include <cstdio>

class BinaryReader
{
private:
	FILE* reader;
	char curByte;
	int bitPos;

	//Buffer
	size_t bufferSize;
	const size_t maxBufferSize = 4194304;//33554432
	char* buffer;
	size_t bufferIndex;
	size_t fileLength;
	size_t fileIndex;

	char ReadFromBuffer();
public:
	bool IsOpened();
	bool IsEOF();
	int FileLength();
	void Reset();
	void EmptyByte();
	char ReadBit();
	char ReadByte();
	int ReadInt();
	char* ReadName();
	void CompleteByte();

	BinaryReader(const char* fileName);
	BinaryReader(FILE* file);
	~BinaryReader();
};

