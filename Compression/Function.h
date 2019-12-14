#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "BinaryReader.h"
using namespace std;

struct Node
{
	char data;
	unsigned int freq;
	Node* left, * right;
};

//Hàm hoán vị 2 Node
void Swap(Node*& a, Node*& b);
//Hàm đếm tần suất xuất hiện của các ký tự trong file và 
//trả về một vector các Node
vector<Node*> CountFrequency(BinaryReader& reader);