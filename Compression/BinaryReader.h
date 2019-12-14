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

	//Đọc 1 ký tự từ buffer
	char ReadFromBuffer();
public:
	//Kiểm tra file đã mở chưa
	bool IsOpened();
	//Kiểm tra đã đọc hết file chưa
	bool IsEOF();
	//Hàm tính độ dài của file
	long long FileLength();
	//Hàm khôi phục trạng thái về như ban đầu
	void Reset();
	//Kiểm tra đã đọc hết bit của byte đang xét chưa, nếu rồi thì đọc byte tiếp theo
	void EmptyByte();
	//Đọc 1 bit từ file
	char ReadBit();
	//Đọc 1 byte từ file
	char ReadByte();
	//Đọc 1 int từ file
	int ReadInt();
	//Đọc một chuỗi tên từ file kết thúc chuỗi bằng ‘\0’
	char* ReadName();
	//Bỏ qua các bit còn lại và tiếp tục đọc byte tiếp theo
	void CompleteByte();

	BinaryReader(const char* fileName);
	BinaryReader(FILE* file);
	~BinaryReader();
};

