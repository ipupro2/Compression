#include "MinHeap.h"

#define Dictionary vector<Word>

struct Word
{
	char key;
	string bits;
};

Node* CreateNode(char data, int freq);
bool IsLeaf(Node* node);
Node* BuildHuffmanTree(vector<Node*>& datas);
void BuildDictionary(Node* root, Dictionary& dict, string cur = "");
void PrintDictionary(Dictionary& dict);
string LookUpDictionary(Dictionary& dict, char c, int low, int high);
void SortDictionary(Dictionary& dict, int low, int high);
bool Traverse(Node* dict, string& s, char& c, int index = 0);
void PrintTree(Node* node);