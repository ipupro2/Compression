#include "FolderProcessing.h"
#include <chrono>
using namespace chrono;

int main()
{
	while (1)
	{
		cout << "Choose options: \n";
		cout << "1. Compress file\n";
		cout << "2. Compress folder\n";
		cout << "3. Extract\n";
		cout << "4. Exit\n";
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
			if (s2[0] == '\0')
			{
				strcpy_s(s2, 100, s1);
				strcat_s(s2, 100, ".tz");
			}
			auto start = high_resolution_clock::now();
			CompressFile(s1, s2);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Operation tooks " << duration.count() << " microseconds\n";

			delete[]s1;
			delete[]s2;
		}
		else if(option == 2)
		{
			char* s1 = new char[100];
			char* s2 = new char[100];
			cout << "Input name: ";
			cin.ignore(1);
			cin.getline(s1, 100);
			strcat_s(s1, 100, "/*");
			cout << "Output file: ";
			cin.getline(s2, 100);
			if (s2[0] == '\0')
			{
				strcpy_s(s2, 100, s1);
				strcat_s(s2, 100, ".tz");
			}
			CompressFolder(s1, s2);
			delete[]s1;
			delete[]s2;
		}
		else if(option == 3)
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
		else if(option == 4)
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