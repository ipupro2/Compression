#pragma once
#include "Function.h"


class MinHeap
{
	vector<Node*> datas;
	int size;

public:
	//Hàm trả về kích thước của heap
	int Size();
	//Hàm hiệu chỉnh heap thành MinHeap
	void Heapify(int pos);
	//Hàm thêm 1 node vào heap
	void Insert(Node* data);
	//Hàm lấy node nhỏ nhất (tức node root) khỏi heap 
	Node* ExtractMin();

	MinHeap();
	MinHeap(vector<Node*>& inData);
};