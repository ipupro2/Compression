#include "Compress.h"

int main()
{
	while (1)
	{
		cout << "Choose options: \n";
		cout << "1. Compress\n";
		cout << "2. Extract\n";
		cout << "3. Exit\n";
		int option;
		cin >> option;
		if (option == 1)
		{
			char* s1 = new char[100];
			char* s2 = new char[100];
			cout << "Input name: ";
			cin.ignore(1);
			cin.getline(s1, 100);
			cout << "Output file: ";
			cin.getline(s2, 100);
			Compress(s1, s2);
			cout << "Compressed!!\n";
			delete[]s1;
			delete[]s2;
		}
		else if(option == 2)
		{
			char* s = new char[100];
			cout << "Input name: ";
			cin.ignore(1);
			cin.getline(s, 100);
			Decompress(s);
			cout << "Extracted!!\n";
			delete[]s;
		}
		else
		{
			exit(0);
		}
	}
	return 0;
}