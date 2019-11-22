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
	fread(&a, sizeof(int), 1, reader);
	bitPos = -1;
	return a;
}

char* BinaryReader::ReadName()
{
	char* s = new char[1000];
	int i;
	CompleteByte();
	fread(&curByte, 1, 1, reader);
	for (i = 0; curByte != '\0'; i++)
	{
		s[i] = curByte;
		fread(&curByte, 1, 1, reader);
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