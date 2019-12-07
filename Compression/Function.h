#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "BinaryReader.h"
using namespace std;

struct Node
{
	char data;
	unsigned int freq;
	Node* left, * right;
};

void Swap(Node*& a, Node*& b);
vector<Node*> CountFrequency(BinaryReader& reader);