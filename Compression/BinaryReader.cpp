#include "BinaryReader.h"

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

void BinaryReader::EmptyByte()
{
	if (bitPos == -1)
	{
		bitPos = 7;
		fread(&curByte, 1, 1, reader);
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
			if ((curByte >> bitPos) & 1)
				c |= 1 << i;
			bitPos--;
			EmptyByte();
		}
	}
	return c;
}
int BinaryReader::ReadInt()
{
	int a;
	fread(&a, sizeof(int), 1, reader);
	bitPos = 7;
	fread(&curByte, 1, 1, reader);
	return a;
}
void BinaryReader::CompleteByte()
{
	if (bitPos == 7)
		return;
	bitPos = 7;
	fread(&curByte, 1, 1, reader);
}
BinaryReader::BinaryReader(const char* fileName)
{
	fopen_s(&reader, fileName, "rb");
	bitPos = 7;
	if (reader)
		fread(&curByte, 1, 1, reader);
	else
		curByte = 0;
}

BinaryReader::BinaryReader(FILE* file)
{
	reader = file;
	bitPos = 7;
	fread(&curByte, 1, 1, reader);
}

BinaryReader::~BinaryReader()
{
	fclose(reader);
}