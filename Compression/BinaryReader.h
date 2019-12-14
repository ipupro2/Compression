#pragma once
#include <cstdio>

class BinaryReader
{
private:
	FILE* reader;
	char curByte;
	int bitPos;

	//Buffer
	long long bufferSize;
	const long long maxBufferSize = 4194304;//33554432
	char* buffer;
	long long bufferIndex;
	long long fileLength;
	long long fileIndex;

	char ReadFromBuffer();
public:
	bool IsOpened();
	bool IsEOF();
	long long FileLength();
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

