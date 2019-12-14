#include "MinHeap.h"

Node* CreateNode(char data, int freq);
bool IsLeaf(Node* node);
Node* BuildHuffmanTree(vector<Node*>& datas);
void DeleteTree(Node* root);
void BuildCodeBook(Node* root, vector<string>& codeBook, string cur = "");
bool Traverse(Node*& node, char& bit, char& c);