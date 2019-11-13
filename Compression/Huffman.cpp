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
	Node* left, *right, *top;
	while (heap.Size() != 1)
	{
		left = heap.ExtractMin();
		right = heap.ExtractMin();
		top = CreateNode('i', left->freq + right->freq);
		top->left = left;
		top->right = right;
		heap.Insert(top);
	}
	return heap.ExtractMin();
}

string LookUpDictionary(Dictionary& dict, char c, int low, int high)
{
	if (low > high)
		return "";
	int mid = (low + high) / 2;
	if (dict[mid].key == c)
		return dict[mid].bits;
	if (dict[mid].key > c)
		return LookUpDictionary(dict, c, low, mid - 1);
	return LookUpDictionary(dict, c, mid + 1, high);
}

void BuildDictionary(Node* root, Dictionary& dict, string cur)
{
	if (root == NULL)
		return;
	if (IsLeaf(root))
	{
		Word word;
		word.key = root->data;
		word.bits = cur;
		dict.push_back(word);
		return;
	}
	BuildDictionary(root->left, dict, cur + "0");
	BuildDictionary(root->right, dict, cur + "1");
}

void Swap(Word& a, Word& b)
{
	Word temp = a;
	a = b;
	b = temp;
}

int Partition(Dictionary& dict, int low, int high)
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

void SortDictionary(Dictionary& dict, int low, int high)
{
	if (low >= high)
		return;
	int pivot = Partition(dict, low, high);
	SortDictionary(dict, low, pivot - 1);
	SortDictionary(dict, pivot + 1, high);
}

bool Traverse(Node* dict, string& s, char& c, int index)
{
	if (dict == NULL)
		return 0;
	if (dict->left == NULL && dict->right == NULL)
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
	if (s[index] == '0')
		return Traverse(dict->left, s, c, index + 1);
	return Traverse(dict->right, s, c, index + 1);
}

void PrintDictionary(Dictionary& dict)
{
	for (Word word : dict)
	{
		cout << word.key << ": " << word.bits << "\n";
	}
}