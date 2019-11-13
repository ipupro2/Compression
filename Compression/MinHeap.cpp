#include "MinHeap.h"

int MinHeap::Size()
{
	return this->size;
}

void MinHeap::Heapify(int pos)
{
	if (2 * pos >= size - 1)
		return;
	int left = 2 * pos + 1;
	int right = 2 * pos + 2;
	int index = left;
	if (right < size && datas[right]->freq < datas[left]->freq)
		index = right;
	if (datas[pos]->freq > datas[index]->freq)
	{
		Swap(datas[pos], datas[index]);
		Heapify(index);
	}
}

void MinHeap::Insert(Node* data)
{
	datas.push_back(data);
	for (int i = datas.size() - 1; i > 0; i = (i - 1) / 2)
	{
		if (datas[i]->freq < datas[(i - 1) / 2]->freq)
			Swap(datas[i], datas[(i - 1) / 2]);
		else
			break;
	}
	size++;
}

//Trả về giá trị nhỏ nhất trong min heap(tức root) và loại nó ra khỏi heap
Node* MinHeap::PopMin()
{
	Node* node = datas[0];
	size--;
	Swap(datas[0], datas[size]);
	datas.pop_back();

	Heapify(0);
	return node;
}

MinHeap::MinHeap()
{
	size = 0;
}

MinHeap::MinHeap(vector<Node*>& inData)
{
	size = inData.size();
	datas = inData;
	for (int i = size / 2 - 1; i >= 0; i--)
		Heapify(i);
}