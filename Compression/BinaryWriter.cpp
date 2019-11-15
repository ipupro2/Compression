#include "BinaryWriter.h"

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
		fwrite(&curByte, 1, 1, writer);
		curByte = 0;
	}
}

//Ghi 1 bit vào byte tạm nếu đủ 8 bit thì bắt đầu ghi vào file
void BinaryWriter::WriteBit(char c)
{
	if(c==1)
		curByte |= 1 << bytePos;
	bytePos--;
	FullByte();
}

//Hàm này dùng để ghi số int vào file
//nhưng chỉ tối ưu cho câu trúc đang sử dụng bên huffman của project này
void BinaryWriter::WriteInt(int c)
{
	WriteRemain();
	fwrite(&c, sizeof(int), 1, writer);
}

void BinaryWriter::WriteByte(char c)
{
	if (bytePos == 7)
	{
		fwrite(&c, 1, 1, writer);
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
	fwrite(&curByte, 1, 1, writer);
	bytePos = 7;
	curByte = 0;
}

BinaryWriter::BinaryWriter(const char* fileName)
{
	fopen_s(&writer, fileName, "wb");
	curByte = 0;
	bytePos = 7;
}

BinaryWriter::BinaryWriter(FILE*& file)
{
	writer = file;
	curByte = 0;
	bytePos = 0;
}

BinaryWriter::~BinaryWriter()
{
	WriteRemain();
	fclose(writer);
}