#include "FolderProcessing.h"
#include <chrono>
using namespace chrono;

int main()
{
	while (1)
	{
		cout << "Choose option: \n";
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
			cout << "Operation took " << duration.count() << " microseconds\n";

			delete[]s1;
			delete[]s2;
		}
		else if(option == 2)
		{
			char* s1 = new char[1000];
			char* s2 = new char[1000];
			cout << "Input name: ";
			cin.ignore(1);
			cin.getline(s1, 1000);
			strcat_s(s1, 1000, "/*");
			cout << "Output file: ";
			cin.getline(s2, 1000);
			if (s2[0] == '\0')
			{
				strcpy_s(s2, 1000, s1);
				strcat_s(s2, 1000, ".tz");
			}
			auto start = high_resolution_clock::now();
			BinaryWriter writer(s2);
			CompressFolder(s1, writer);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Operation took " << duration.count() << " microseconds\n";
			delete[]s1;
			delete[]s2;
		}
		else if(option == 3)
		{
			char* s1 = new char[1000];
			char* s2 = new char[1000];
			cout << "Input name: ";
			cin.ignore(1);
			cin.getline(s1, 1000);
			cout << "Output folder: ";
			cin.getline(s2, 1000);
			auto start = high_resolution_clock::now();
			BinaryReader reader(s1);
			Decompress(s2, reader, true);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Operation took " << duration.count() << " microseconds\n";

			delete[]s1;
		}
		else if(option == 4)
		{
			exit(0);
		}
		else
		{
			cout << "Wrong input!!!\n";
		}
		cout << "\a---------------------------\n";
	}
	return 0;
}