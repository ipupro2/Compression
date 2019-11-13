#pragma once
#include "Function.h"


class MinHeap
{
	vector<Node*> datas;
	int size;

public:
	int Size();
	void Heapify(int pos);
	void Insert(Node* data);
	Node* ExtractMin();
	MinHeap();
	MinHeap(vector<Node*>& inData);
};