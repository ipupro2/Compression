#include "BinaryWriter.h"

void BinaryWriter::WriteToBuffer(const char& c)
{
	buffer[bufferIndex++] = c;
	//Nếu kích thước buffer đã đạt đến ngưỡng thì ghi nó vào file
	if (bufferIndex >= maxBufferSize)
	{
		fwrite(buffer, maxBufferSize, 1, writer);
		bufferIndex = 0;
	}
}

bool BinaryWriter::IsOpened()
{
	return writer;
}

char BinaryWriter::RemainBits()
{
	return bytePos;
}

void BinaryWriter::FullByte()
{
	if (bytePos == -1)
	{
		bytePos = 7;
		WriteToBuffer(curByte);
		curByte = 0;
	}
}

//Ghi 1 bit vào byte tạm nếu đủ 8 bit thì bắt đầu ghi vào file
void BinaryWriter::WriteBit(const char& c)
{
	if(c==1)
		curByte |= 1 << bytePos;
	bytePos--;
	FullByte();
}

void BinaryWriter::WriteInt(const int& c)
{
	WriteRemain();
	//Ghi 4 byte của c vào file
	WriteToBuffer((char)(c & 0xFF));
	WriteToBuffer((char)((c >> 8) & 0xFF));
	WriteToBuffer((char)((c >> 16) & 0xFF));
	WriteToBuffer((char)((c >> 24) & 0xFF));
}

void BinaryWriter::WriteByte(const char& c)
{
	if (bytePos == 7)
	{
		WriteToBuffer(c);
		return;
	}
	for (int i = 7; i >= 0; i--)
	{
		if ((c >> i) & 1)
			curByte |= 1 << bytePos;
		bytePos--;
		FullByte();
	}
}

void BinaryWriter::WriteRemain()
{
	if (bytePos == 7)
		return;
	WriteToBuffer(curByte);
	bytePos = 7;
	curByte = 0;
}

BinaryWriter::BinaryWriter(const char* fileName)
{
	writer = fopen(fileName, "wb");
	if (writer)
	{
		buffer = new char[maxBufferSize];
	}
	else
	{
		buffer = NULL;
	}
	bufferIndex = 0;
	curByte = 0;
	bytePos = 7;
}

void BinaryWriter::WriteBuffer()
{
	if (bufferIndex > 0)
		fwrite(buffer, bufferIndex, 1, writer);
	bufferIndex = 0;
}

BinaryWriter::BinaryWriter(FILE*& file)
{
	writer = file;
	if (writer)
	{
		buffer = new char[maxBufferSize];
	}
	else
	{
		buffer = NULL;
	}
	bufferIndex = 0;
	bufferIndex = 0;
	curByte = 0;
	bytePos = 0;
}

BinaryWriter::~BinaryWriter()
{
	WriteRemain();
	WriteBuffer();
	if(buffer)
		delete[] buffer;
	if(writer)
		fclose(writer);
}