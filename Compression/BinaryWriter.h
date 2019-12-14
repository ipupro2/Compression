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
	//Ghi ký tự c vào buffer, khi buffer đầy sẽ ghi nó vào file
	void WriteToBuffer(const char& c);

public:
	//Kiểm tra file đã mở chưa
	bool IsOpened();
	//Đếm số bit còn thiếu trong byte đang xử lý
	char RemainBits();
	//Kiểm tra đã ghi đủ 8 bit chưa, nếu đủ thì ghi byte này vào file
	void FullByte();
	//Ghi 1 bit vào file với c là 0 hoặc 1
	void WriteBit(const char& c);
	//Ghi 1 byte bất kỳ vào file
	void WriteByte(const char& c);
	//Ghi 1 số nguyên vào file
	void WriteInt(const int& c);
	//Ghi các bit còn lại vào file, các bit còn thiếu sẽ là 0
	void WriteRemain();

	//Ghi phần còn lại của buffer vào file
	void WriteBuffer();

	BinaryWriter(const char* fileName);
	BinaryWriter(FILE* &file);
	~BinaryWriter();
};

