#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
using namespace std;

struct Node
{
	char data;
	unsigned int freq;
	Node* left, * right;
};

void Swap(Node*& a, Node*& b);
bool IsIn(vector<Node>& datas, char c);
vector<Node*> CountFrequency(FILE*& inFile);