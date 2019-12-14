#include "Huffman.h"

//Tạo một node mới với dữ liệu là data và tần số là freq
Node* CreateNode(char data, int freq)
{
	Node* node = new Node();
	node->data = data;
	node->freq = freq;
	node->left = node->right = NULL;
	return node;
}

//Kiểm tra 1 node có phải node lá không
bool IsLeaf(Node* node)
{
	return node->left == NULL;
}

//Xây dựng cây huffman từ vector các Node
Node* BuildHuffmanTree(vector<Node*>& datas)
{
	MinHeap heap(datas);
	Node* left, * right, * top;
	while (heap.Size() != 1)
	{
		left = heap.ExtractMin();
		right = heap.ExtractMin();
		top = CreateNode(NULL, left->freq + right->freq);
		top->left = left;
		top->right = right;
		heap.Insert(top);
	}
	return heap.ExtractMin();
}

//Hủy cấp phát các node của cây
void DeleteTree(Node* root)
{
	if (root == NULL)
		return;
	DeleteTree(root->left);
	DeleteTree(root->right);
	delete root;
}

//Xây dựng codebook bằng cách duyệt cây huffman và lưu vào biến codeBook
//Hàm này duyệt tất cả các node của cây nhưng ưu tiên bên trái trước
void BuildCodeBook(Node* root, vector<string>& codeBook, string cur)
{
	if (root == NULL)
		return;
	//Nếu đây là nút ra thì tạo thêm 1 codebook mới
	if (IsLeaf(root))
	{
		codeBook[(unsigned char)root->data] = cur;
		return;
	}

	BuildCodeBook(root->left, codeBook, cur + "0");
	BuildCodeBook(root->right, codeBook, cur + "1");
}

//Duyệt cây Huffman để tìm node lá theo bit, sau đó gán cho c
bool Traverse(Node*& node, char& bit, char& c)
{
	if (node == NULL)
		return 0;
	//Nếu bit này là 0 thì đi sang trái, 1 thì đi sáng phải
	if (bit == 0)
		node = node->left;
	else
		node = node->right;
	//Nếu đi đến node lá thì gán cho c dữ liệu node lá
	if (IsLeaf(node))
	{
		c = node->data;
		return 1;
	}
	return 0;
}