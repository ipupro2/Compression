#include "FolderProcessing.h"

void CompressFolder(const char* folderName, const char* outFileName)
{
	int nameLength = strlen(folderName);
	char* directory = new char[100];
	strcpy_s(directory, 100, folderName);
	vector<char*> directoriesNames;
	vector<char*> fileNames;
	WIN32_FIND_DATA	find;
	HANDLE hFind = FindFirstFile(directory, &find);

	directory[nameLength - 1] = '\0';
	if (hFind == INVALID_HANDLE_VALUE)
	{
		cout << "Cannot open directories!";
		FindClose(hFind);
		return;
	}
	do
	{
		char* temp1 = new char[100];
		strcpy_s(temp1, 100, find.cFileName);
		if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(temp1, ".") != 0 && strcmp(temp1, ".."))
				directoriesNames.push_back(temp1);
		}
		else
		{
			fileNames.push_back(temp1);
		}
	} while (FindNextFile(hFind, &find));
	FindClose(hFind);

	BinaryWriter writer(outFileName);

	writer.WriteByte('t');
	writer.WriteByte('z');
	writer.WriteInt(fileNames.size());

	for (int i = 0; i < fileNames.size(); i++)
	{
		Compress(fileNames[i], writer, directory);
		writer.WriteRemain();
	}
}