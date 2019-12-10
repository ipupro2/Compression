#include "Compress.h"

char* GetFileName(const char* s)
{
	int i, length = strlen(s);
	for (i = length - 1; i >= 0; i--)
	{
		if (s[i] == '\\' || s[i] == '/')
			break;
	}
	char* result = new char[length + 1];
	result[length - i - 1] = '\0';
	if (length > 0)
	{
		for (int j = i + 1; j < length; j++)
		{
			result[j - i - 1] = s[j];
		}
	}
	return result;
}

void EncodeDescription(BinaryWriter& writer, const char* inFileName, vector<Node*>& datas)
{
	char* name = GetFileName(inFileName);
	int nameLength = strlen(name);
	for (int i = 0; i <= nameLength; i++)
	{
		writer.WriteByte(name[i]);
	}
	delete[] name;
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
	
	vector<string> codeBook;
	codeBook.resize(256);
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
		code = codeBook[(unsigned char)c];
		for (int i = 0; i < code.size(); i++)
		{
			length++;
			writer.WriteBit(code[i] - '0');
		}
		c = reader.ReadByte();
	}
	DeleteTree(node);
}

void Compress(const char* inFileName, BinaryWriter &writer, const char* directory)
{
	char* filePath = new char[1000];
	strcpy_s(filePath, 1000, directory);
	strcat_s(filePath, 1000, inFileName);
	BinaryReader reader(filePath);
	if (!(reader.IsOpened() && writer.IsOpened()))
	{
		cout << "File's not exist\n";
		return;
	}

	vector<Node*> datas = CountFrequency(reader);

	EncodeDescription(writer, inFileName, datas);

	//Xử lý file rỗng
	if (datas.size() == 0)
	{
		writer.WriteBit(1);
		writer.WriteByte(0);
		writer.WriteInt(0);
		return;
	}

	EncodeFileData(reader, writer, datas);
}

void CompressFile(const char* inFileName, const char* outFileName)
{
	BinaryWriter writer(outFileName);
	writer.WriteByte('t');
	writer.WriteByte('z');
	writer.WriteInt(0);
	writer.WriteInt(1);
	Compress(inFileName, writer, "");
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

void Decompress(const char* folder, BinaryReader& reader, bool header)
{
	if (!reader.IsOpened())
		return;
	if (!(CreateDirectory(folder, NULL) || ERROR_ALREADY_EXISTS == GetLastError()))
	{
		cout << "Folder is not existed or accessible!\n";
		return;
	}
	//Kiểm tra đây có phải header của file nén
	if (header)
	{
		for (int i = 0; i < 2; i++)
		{
			if (reader.ReadByte() != "tz"[i])
			{
				cout << "Wrong format, cannot decompress!!\n";
				return;
			}
		}
	}
	int nFolder = reader.ReadInt();
	for (int i = 0; i < nFolder; i++)
	{
		char* folderName = reader.ReadName();
		char* folderPath = new char[1000];
		strcpy_s(folderPath, 1000, folder);
		int length = strlen(folderPath);
		if(length > 0 && folderPath[length - 1]!='/')
			strcat_s(folderPath, 1000, "/");
		strcat_s(folderPath, 1000, folderName);
		Decompress(folderPath, reader, false);
		delete[] folderName;
		delete[] folderPath;
	}
	int nFile = reader.ReadInt();
	for (int fileIndex = 0; fileIndex < nFile; fileIndex++)
	{
		//Read name
		char* outFileName = reader.ReadName();
		char* filePath = new char[1000];
		strcpy_s(filePath, 1000, folder);
		int pathLength = strlen(filePath);
		if (pathLength > 0 && filePath[pathLength - 1] != '/')
			strcat_s(filePath, 1000, "/");
		strcat_s(filePath, 1000, outFileName);

		BinaryWriter writer(filePath);
		if (!writer.IsOpened())
		{
			cout << "Cannot write file " << outFileName << "\n";
			return;
		}
		delete[] outFileName;
		delete[] filePath;
		Node* node = DecodeTree(reader);
		
		//Đọc số lượng ký tự có trong file gốc
		int length = reader.ReadInt();

		char temp = 0;
		char c;
		Node* tempNode = node;
		if (length != 0)
		{
			c = reader.ReadBit();
			while (!reader.IsEOF())
			{
				if (Traverse(tempNode, c, temp))
				{
					tempNode = node;
					writer.WriteByte(temp);
					length--;
				}
				if (length == 0)
					break;
				c = reader.ReadBit();
			}
			writer.WriteRemain();
			if (length != 0)
				cout << "File is corrupted!\n";
		}
		DeleteTree(node);
		reader.CompleteByte();
	}
}