#include "BinaryReader.h"

char BinaryReader::ReadFromBuffer()
{
	if (bufferSize == 0)
	{
		fread(buffer, 1, 1, reader);
		return 0;
	}
	char c = buffer[bufferIndex++];
	if (bufferIndex >= bufferSize && ! IsEOF())
	{
		bufferSize = (fileLength - fileIndex) < maxBufferSize ? (fileLength - fileIndex) : maxBufferSize;
		bufferIndex = 0;
		fileIndex += bufferSize;
		if (bufferSize > 0)
			fread(buffer, bufferSize, 1, reader);
	}
	return c;
}

bool BinaryReader::IsOpened()
{
	return reader;
}

bool BinaryReader::IsEOF()
{
	return feof(reader);
}

int BinaryReader::FileLength()
{
	int curPos = ftell(reader);
	fseek(reader, 0, SEEK_END);
	int length = ftell(reader);
	fseek(reader, curPos, SEEK_SET);
	return length;
}

void BinaryReader::Reset()
{
	fseek(reader, 0, SEEK_SET);
	bitPos = 7;
	bufferIndex = 0;
	bufferSize = fileLength < maxBufferSize ? fileLength : maxBufferSize;
	fileIndex = bufferSize;
	fread(buffer, bufferSize, 1, reader);
	curByte = ReadFromBuffer();
}

void BinaryReader::EmptyByte()
{
	if (bitPos == -1)
	{
		bitPos = 7;
		curByte = ReadFromBuffer();
	}
}
char BinaryReader::ReadBit()
{
	EmptyByte();
	char c = (curByte >> bitPos) & 1;
	bitPos--;
	return c;
}
char BinaryReader::ReadByte()
{
	EmptyByte();
	char c = 0;
	if (bitPos == 7)
	{
		bitPos = -1;
		c = curByte;
	}
	else
	{
		for (int i = 7; i >= 0; i--)
		{
			EmptyByte();
			if ((curByte >> bitPos) & 1)
				c |= 1 << i;
			bitPos--;
		}
	}
	return c;
}
int BinaryReader::ReadInt()
{
	int a;
	a = (unsigned char)ReadFromBuffer();
	a |= (((unsigned char)ReadFromBuffer()) << 8);
	a |= (((unsigned char)ReadFromBuffer()) << 16);
	a |= (((unsigned char)ReadFromBuffer()) << 24);
	bitPos = -1;
	return a;
}

char* BinaryReader::ReadName()
{
	char* s = new char[1000];
	int i;
	CompleteByte();
	curByte = ReadFromBuffer();
	for (i = 0; curByte != '\0'; i++)
	{
		s[i] = curByte;
		curByte = ReadFromBuffer();
	}
	bitPos = -1;
	s[i] = '\0';
	return s;
}

void BinaryReader::CompleteByte()
{
	if (bitPos == 7)
		return;
	bitPos = -1;
}
BinaryReader::BinaryReader(const char* fileName)
{
	fopen_s(&reader, fileName, "rb");
	bitPos = 7;
	if (reader)
	{
		fileLength = FileLength();
		bufferIndex = 0;
		bufferSize = fileLength < maxBufferSize ? fileLength : maxBufferSize;
		buffer = new char[bufferSize];
		fileIndex = bufferSize;
		fread(buffer, bufferSize, 1, reader);
		curByte = ReadFromBuffer();
	}
	else
	{
		fileLength = 0;
		bufferIndex = 0;
		bufferSize = 0;
		buffer = NULL;
		curByte = 0;
		fileIndex = 0;
	}
}

BinaryReader::BinaryReader(FILE* file)
{
	reader = file;
	bitPos = 7;
	if (reader)
	{
		fileLength = FileLength();
		bufferIndex = 0;
		bufferSize = fileLength < maxBufferSize ? fileLength : maxBufferSize;
		buffer = new char[bufferSize];
		fileIndex = bufferSize;
		fread(buffer, bufferSize, 1, reader);
		curByte = ReadFromBuffer();
	}
	else
	{
		fileLength = 0;
		bufferIndex = 0;
		bufferSize = 0;
		buffer = NULL;
		curByte = 0;
		fileIndex = 0;
	}
}

BinaryReader::~BinaryReader()
{
	if (buffer)
		delete[] buffer;
	if(reader)
		fclose(reader);
}