#include "Function.h"

void Swap(Node*& a, Node*& b)
{
	Node* temp = a;
	a = b;
	b = temp;
}

//Hàm đếm tần suất xuất hiện của các ký tự trong file và trả về một vector các node của cây Huffman
vector<Node*> CountFrequency(BinaryReader& reader)
{
	vector<Node*> datas;
	unsigned char c = 0;

	//Để tối ưu tốc độ đọc file, tạo trước 1 vector của tần suất
	//có 256 phần tử tương ứng 256 ký tự khác nhau của 1 bytes
	vector<int> freqs(256);
	c = reader.ReadByte();
	//Đọc hết file và tăng tần suất tại địa điểm c
	while (!reader.IsEOF())
	{
		freqs[(int)c]++;
		c = reader.ReadByte();
	}
	datas.reserve(256);
	for (int i = 0; i < 256; i++)
	{
		//Nếu tần suất của ký tự freqs[i] là khác 0, tức có xuất hiện trong file
		// thì ta tạo một node(của cây huffman) và đưa vào vector datas(tức vector kết quả)
		if (freqs[i] != 0)
		{
			Node* node = new Node({ (char)i, (unsigned int)freqs[i],NULL,NULL });
			datas.push_back(node);
		}
	}
	reader.Reset();
 	return datas;
}