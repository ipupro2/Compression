#include "Huffman.h"

Node* CreateNode(char data, int freq)
{
	Node* node = new Node();
	node->data = data;
	node->freq = freq;
	node->left = node->right = NULL;
	return node;
}

bool IsLeaf(Node* node)
{
	return (node->left == NULL && node->right == NULL);
}

void PrintTree(Node* node)
{
	if (node == NULL)
		return;
	cout << node->data << ": " << node->freq << "\n";
	PrintTree(node->left);
	PrintTree(node->right);
}

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

void DeleteTree(Node* root)
{
	if (root == NULL)
		return;
	DeleteTree(root->left);
	DeleteTree(root->right);
	delete root;
}

//Chuỗi bit cho c trong codebook bằng tìm nhị phân
string LookUpCodeBook(CodeBook& dict, char c, int low, int high)
{
	if (low > high)
		return "";
	int mid = (low + high) / 2;
	if (dict[mid].key == c)
		return dict[mid].bits;
	if (dict[mid].key > c)
		return LookUpCodeBook(dict, c, low, mid - 1);
	return LookUpCodeBook(dict, c, mid + 1, high);
}

//Xây dựng codebook bằng cách duyệt cây huffman và lưu vào biến dict
//Hàm này duyệt tất cả các node của cây nhưng ưu tiên bên trái trước
void BuildCodeBook(Node* root, CodeBook& dict, string cur)
{
	if (root == NULL)
		return;
	//Nếu đây là nút ra thì tạo thêm 1 codebook mới
	if (IsLeaf(root))
	{
		Word word;
		word.key = root->data;
		word.bits = cur;
		dict.push_back(word);
		return;
	}

	BuildCodeBook(root->left, dict, cur + "0");
	BuildCodeBook(root->right, dict, cur + "1");
}

void Swap(Word& a, Word& b)
{
	Word temp = a;
	a = b;
	b = temp;
}

int Partition(CodeBook& dict, int low, int high)
{
	Word pivot = dict[high];
	int i = low - 1;
	int j;
	for (j = low; j < high; j++)
	{
		if (dict[j].key < pivot.key)
		{
			i++;
			Swap(dict[i], dict[j]);
		}
	}
	Swap(dict[i + 1], dict[high]);
	return i + 1;
}

//Sắp xếp code book theo thuật toán Quicksort
void SortCodeBook(CodeBook& dict, int low, int high)
{
	if (low >= high)
		return;
	int pivot = Partition(dict, low, high);
	SortCodeBook(dict, low, pivot - 1);
	SortCodeBook(dict, pivot + 1, high);
}

//Duyệt cây Huffman để tìm node lá theo chuỗi bit s, sau đó gán cho c

//index là vị trí đang xét trong chuỗi bit, 
//nếu duyệt hết chuỗi s đúng ngay node lá thì đó chính là giá trị cần tìm
bool Traverse(Node* dict, string& s, char& c, int index)
{
	if (dict == NULL)
		return 0;
	if (IsLeaf(dict))
	{
		if (index == s.length())
		{
			c = dict->data;
			return 1;
		}
		return 0;
	}
	if (index >= s.length())
		return 0;
	if (s[index] == 0)
		return Traverse(dict->left, s, c, index + 1);
	return Traverse(dict->right, s, c, index + 1);
}

//For debug only
void PrintCodeBook(CodeBook& dict)
{
	for (Word word : dict)
	{
		cout << word.key << ": " << word.bits << "\n";
	}
}