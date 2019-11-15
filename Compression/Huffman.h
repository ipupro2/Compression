#include "MinHeap.h"

#define CodeBook vector<Word>

struct Word
{
	char key;
	string bits;
};

Node* CreateNode(char data, int freq);
bool IsLeaf(Node* node);
Node* BuildHuffmanTree(vector<Node*>& datas);
void DeleteTree(Node* root);
void BuildCodeBook(Node* root, CodeBook& dict, string cur = "");
void PrintCodeBook(CodeBook& dict);
string LookUpCodeBook(CodeBook& dict, char c, int low, int high);
void SortCodeBook(CodeBook& dict, int low, int high);
bool Traverse(Node* dict, string& s, char& c, int index = 0);
void PrintTree(Node* node);