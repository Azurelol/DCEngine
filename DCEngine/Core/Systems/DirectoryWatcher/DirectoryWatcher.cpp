// DirectoryWatcher.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>
#include <dirent.h>
#include <stdio.h>
#include <string> 
#include <vector>
#include <iostream>

struct FileInfo
{

	SYSTEMTIME time;
	std::string directory;
	std::string name;

	FileInfo(SYSTEMTIME Time, std::string Directory, std::string Name)
	{
		name = Name;
		directory = Directory;
		time = Time;
	}

	bool operator==(const FileInfo& rhs)
	{
		return ((time.wSecond == rhs.time.wSecond) && (time.wMinute == rhs.time.wMinute) && (time.wHour == rhs.time.wHour) && (time.wDay == rhs.time.wDay) &&
			(time.wMonth == rhs.time.wMonth) && (time.wYear == rhs.time.wYear) && (name == rhs.name));
	}
	bool operator!=(const FileInfo& rhs)
	{
		return !((time.wSecond == rhs.time.wSecond) && (time.wMinute == rhs.time.wMinute) && (time.wHour == rhs.time.wHour) && (time.wDay == rhs.time.wDay) &&
			(time.wMonth == rhs.time.wMonth) && (time.wYear == rhs.time.wYear) && (name == rhs.name));
	}

};

/* Returns a list of files in a directory (except the ones that begin with a dot) */
void GetAllFilesInDirectory(std::vector<FileInfo> & out, std::string const & directory)
{
	HANDLE dir;
	WIN32_FIND_DATA fileData;
	SYSTEMTIME stUTC, stLocal;

	wchar_t wstr[1000];
	mbstowcs(wstr, (directory + "/*").c_str(), 1000);

	if ((dir = FindFirstFile(wstr, &fileData)) == INVALID_HANDLE_VALUE)
		return; // No files found
	do
	{
		if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) // Is directory
			continue;

		std::wstring filename = fileData.cFileName;
		std::string s((const char*)&filename[0], sizeof(wchar_t) / sizeof(char)*filename.size());

		if (filename[0] == '.')
			continue;

		// Convert the last-write time to local time.
		FileTimeToSystemTime(&fileData.ftLastWriteTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

		out.push_back({ stLocal, directory, s });


	} while (FindNextFile(dir, &fileData));

	FindClose(dir);
}

std::string GetFileExtension(std::string path) {
	int indDot = path.find_last_of(".");
	if (indDot == -1) {
		return "";
	}
	else {
		return path.substr(indDot + 1, path.size() - indDot - 1);
	}
}



void CompareFileInfo(std::vector<FileInfo> & currFile, std::vector<FileInfo> & prevFile)
{
	for (unsigned i = 0; i < currFile.size(); i++)
	{
		for (unsigned j = 0; j < prevFile.size(); j++)
		{
			if (currFile[i] == prevFile[j])
			{
				std::cout << currFile[i].name << " is the same!\n";
				break;
			}
			if (j + 1 == prevFile.size())
			{
				std::cout << currFile[i].name << " was changed!\n";
			}
		}
	}
}

int WatchDirectory(std::string directory)
{
	std::vector<FileInfo> prevFileInfo, currFileInfo;

	std::vector<std::string> directoryFileNames, prevDirectoryFileNames;
	std::vector<SYSTEMTIME> directoryFileDates, prevDirectoryFileDates;
	//std::string directory = "C:/Users/Connor/Pictures";

	GetAllFilesInDirectory(currFileInfo, directory);
	for (int i = 0; i < 100000; i++)
	{
		Sleep(5000);
		prevFileInfo = currFileInfo;
		currFileInfo.clear();
		GetAllFilesInDirectory(currFileInfo, directory);
		CompareFileInfo(currFileInfo, prevFileInfo);
		std::cout << "did stuff\n";
	}
	system("pause");
	return 0;
}
