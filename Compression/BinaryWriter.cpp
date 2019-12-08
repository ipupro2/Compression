#include "BinaryWriter.h"

void BinaryWriter::WriteToBuffer(const char& c)
{
	buffer[bufferIndex++] = c;
	if (bufferIndex >= maxBufferSize)
	{
		fwrite(buffer, maxBufferSize, 1, writer);
		bufferIndex = 0;
	}
}

//Kiểm tra file đã mở thành công không
bool BinaryWriter::IsOpened()
{
	return writer;
}

char BinaryWriter::RemainBits()
{
	return bytePos;
}

void BinaryWriter::MoveTo(int index)
{
	fseek(writer, index, SEEK_SET);
}

//Kiểm tra nếu byte hiện tại đủ 8 bit chưa, nếu rồi thì ghi nó vào file
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

//Hàm này dùng để ghi số int vào file
//nhưng chỉ tối ưu cho câu trúc đang sử dụng bên huffman của project này
void BinaryWriter::WriteInt(const int& c)
{
	WriteRemain();
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

//Ghi các bit còn sót lại
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
	fopen_s(&writer, fileName, "wb");
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

void BinaryWriter::WriteBuffer()
{
	if (bufferIndex > 0)
		fwrite(buffer, bufferIndex, 1, writer);
	bufferIndex = 0;
}

BinaryWriter::~BinaryWriter()
{
	WriteRemain();
	WriteBuffer();
	if(buffer)
		delete[] buffer;
	fclose(writer);
}