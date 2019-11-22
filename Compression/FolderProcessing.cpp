#include "FolderProcessing.h"

void CompressFolder(const char* folderName, BinaryWriter& writer, bool header)
{
	int nameLength = strlen(folderName);
	char* directory = new char[1000];
	strcpy_s(directory, 1000, folderName);
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
		char* temp1 = new char[1000];
		strcpy_s(temp1, 1000, find.cFileName);
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

	if (header)
	{
		writer.WriteByte('t');
		writer.WriteByte('z');
	}
	writer.WriteInt(directoriesNames.size());
	for (int i = 0; i < directoriesNames.size(); i++)
	{
		char* temp = new char[1000];
		strcpy_s(temp, 1000, directory);
		strcat_s(temp, 1000, directoriesNames[i]);
		strcat_s(temp, 1000, "/*");
		int length = strlen(directoriesNames[i]);
		for (int j = 0; j <= length; j++)
			writer.WriteByte(directoriesNames[i][j]);
		CompressFolder(temp, writer, false);
		delete[] temp;
	}
	writer.WriteInt(fileNames.size());

	for (int i = 0; i < fileNames.size(); i++)
	{
		Compress(fileNames[i], writer, directory);
		writer.WriteRemain();
	}
}