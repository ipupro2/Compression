#include "Huffman.h"
#include "BinaryWriter.h"
#include "BinaryReader.h"
#include <Windows.h>

//Hàm lấy tên file từ chuỗi đường dẫn s
char* GetFileName(const char* s);
//Ghi lại thông tin của file(chỉ bao gồm tên file) vào file nén
void EncodeDescription(BinaryWriter& writer, const char* inFileName, vector<Node*>& datas);
//Hàm lưu cây lại với node là root của cây
void EncodeTree(BinaryWriter& writer, Node*& node);
//Ghi lại dữ liệu nén vào file
void EncodeFileData(BinaryReader& reader, BinaryWriter& writer, vector<Node*>& datas);
//Giải mã cây Huffman từ file
Node* DecodeTree(BinaryReader& reader);
//Hàm nén 1 file với tên là inFileName nằm ở thư mục directory
void Compress(const char* inFileName, BinaryWriter& writer, const char* directory = "/");
//Hàm xử lý cho việc nén 1 file
void CompressFile(const char* inFileName, const char* outFileName);
//Hàm giải nén 1 file ra 1 folder bất kỳ
void Decompress(const char* folder, BinaryReader& reader, bool header);
