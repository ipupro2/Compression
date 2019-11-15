#include "Compress.h"

void EncodeDescription(BinaryWriter& writer, const char* inFileName, vector<Node*>& datas)
{
	//Nhúng header vào file - nếu nó đúng header thì mới được phép giải nén
	writer.WriteByte('t');
	writer.WriteByte('z');
	int nameLength = strlen(inFileName);
	for (int i = 0; i <= nameLength; i++)
	{
		writer.WriteByte(inFileName[i]);
	}
}

//Hàm lưu cây lại
void EncodeTree(BinaryWriter& writer, Node*& node)
{
	if (IsLeaf(node))
	{
		writer.WriteBit(1);
		writer.WriteByte(node->data);
	}
	else
	{
		writer.WriteBit(0);
		EncodeTree(writer, node->left);
		EncodeTree(writer, node->right);
	}
}

void EncodeFileData(BinaryReader& reader, BinaryWriter& writer, vector<Node*>& datas)
{
	Node* node = BuildHuffmanTree(datas);
	
	unordered_map<char, string> codeBook;
	BuildCodeBook(node, codeBook);

	//Lưu cây lại
	EncodeTree(writer, node);

	writer.WriteInt(node->freq);

	char c;
	string code;
	c = reader.ReadByte();
	int length = 0;
	while (!reader.IsEOF())
	{
		code = codeBook[c];
		for (int i = 0; i < code.size(); i++)
		{
			length++;
			writer.WriteBit(code[i] - '0');
		}
		c = reader.ReadByte();
	}

	DeleteTree(node);
}

void Compress(const char* inFileName, const char* outFileName)
{
	FILE* inFile;
	fopen_s(&inFile, inFileName, "rb");

	BinaryReader reader(inFileName);
	BinaryWriter writer(outFileName);

	if (!(inFile && writer.IsOpened()))
	{
		if (inFile)
			fclose(inFile);
		cout << "File's not exist\n";
		return;
	}

	vector<Node*> datas = CountFrequency(inFile);

	EncodeDescription(writer, inFileName, datas);
	EncodeFileData(reader, writer, datas);

	fclose(inFile);
}

Node* DecodeTree(BinaryReader& reader)
{
	Node* node = new Node;
	if (reader.ReadBit())
	{
		node = CreateNode(reader.ReadByte(), NULL);
		return node;
	}
	node->data = NULL;
	node->left = DecodeTree(reader);
	node->right = DecodeTree(reader);
	return node;
}

void Decompress(const char* fileName)
{
	BinaryReader reader(fileName);
	if (!reader.IsOpened())
		return;
	//Kiểm tra đây có phải header của file nén
	for (int i = 0; i < 2; i++)
	{
		if (reader.ReadByte() != "tz"[i])
		{
			cout << "Wrong format!!";
			return;
		}
	}

	//Read name
	char* outFileName = new char[100], c;
	int i;
	for (i = 0; (c = reader.ReadByte()) && i < 100; i++)
		outFileName[i] = c;
	outFileName[i] = '\0';

	BinaryWriter writer(outFileName);

	delete[] outFileName;
	if(!writer.IsOpened())
		return;

	Node* node = DecodeTree(reader);
	//Đọc số lượng ký tự có trong file gốc
	int length = reader.ReadInt();

	string bits;
	char temp = 0;
	c = reader.ReadBit();
	while (!reader.IsEOF())
	{
		bits += c;
		if (Traverse(node, bits, temp))
		{
			writer.WriteByte(temp);
			bits = "";
			length--;
		}
		if (length == 0)
			break;
		c = reader.ReadBit();
	}
	if (length != 0)
		cout << "File is corrupted!\n";
	DeleteTree(node);
}