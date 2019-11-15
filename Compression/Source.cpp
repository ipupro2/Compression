#include "Compress.h"
#include <chrono>
using namespace chrono;

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

			auto start = high_resolution_clock::now();
			Compress(s1, s2);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Operation tooks " << duration.count() << " microseconds\n";

			delete[]s1;
			delete[]s2;
		}
		else if(option == 2)
		{
			char* s = new char[100];
			cout << "Input name: ";
			cin.ignore(1);
			cin.getline(s, 100);

			auto start = high_resolution_clock::now();
			Decompress(s);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Operation tooks " << duration.count() << " microseconds\n";

			delete[]s;
		}
		else if(option == 3)
		{
			exit(0);
		}
		else
		{
			cout << "Wrong input!!!\n";
		}
		cout << "---------------------------\n";
	}
	return 0;
}