// g++ renamer.cpp -municode -o renamer -lshlwapi -s -O2 -Wall -Wextra

#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <shlwapi.h>
#include <filesystem>

int wmain(int argc, wchar_t **argv)
{
    using namespace std;
    namespace fs = std::filesystem;

    if (argc < 2)
    {
        wcout << L"Renames all files in provided directory like so: name1, name2, name3...\n"
              << L"Run with inputs of a new file name\n"
              << L"An optional second input is the number to start at. Defaults to 1 "
              << L"And an optional third input of a fuzzy search term like test*.txt to search with"
              << endl;
        exit(0);
    }
    int startCount = 1, i;
    wstring fuzzySearch;
    if (argc > 2)
    {
        for (i = 0; argv[2][i] != 0; ++i)
        {
            if (!iswdigit(argv[2][i]))
            {
                wcerr << L"Invalid number provided to third argument" << endl;
                exit(1);
            }
        }
        wchar_t *end;
        startCount = wcstol(argv[2], &end, 10);
        fuzzySearch = argc > 3 ? argv[3] : L"*.*"; // *.* finds all files
    }

    wstring dirPath(fs::current_path().wstring());
    wchar_t last_char = dirPath.back();
    if (last_char != L'/' && last_char != L'\\')
    {
        dirPath += L'/';
    }
    wstring originalPath(dirPath);
    dirPath += fuzzySearch;

    vector<wstring> toRename;
    struct _WIN32_FIND_DATAW dirData;

    wcout << L"Checking if path is valid...\n\n";

    // ask Windows to find any file in provided directory
    HANDLE fileHandle = FindFirstFileW(dirPath.c_str(), &dirData);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        wcout << L"No such path " << argv[1] << endl;
        exit(1);
    }

    do
    {
        // add all non-directories to our list
        if (dirData.dwFileAttributes == FILE_ATTRIBUTE_NORMAL || dirData.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
        {
            toRename.push_back(wstring(dirData.cFileName));
        }
    } while (FindNextFileW(fileHandle, &dirData));

    sort(toRename.begin(), toRename.end(), [](wstring &a, wstring &b)
         { return StrCmpLogicalW(a.c_str(), b.c_str()) == -1; });

    if (toRename.size() > 20)
    {
        wcout << L"Found " << toRename.size() << L" files, are you sure you want to rename? (y/n)\n";
        string response;
        cin >> response;
        if (response != "y" && response != "Y")
        {
            wcout << L"Exiting...";
            exit(0);
        }
        cout << L"Ok then\n";
    }

    int renameStatus;
    for (wstring file : toRename)
    {
        wstring originalFullPath = originalPath + file;
        // construct new name without extention
        wstring newFullName = originalPath + argv[1] + to_wstring(startCount);
        // get file extension
        size_t extLocation = file.find_last_of('.');
        if (extLocation != std::wstring::npos)
            newFullName += file.substr(extLocation);

        renameStatus = _wrename(originalFullPath.c_str(), newFullName.c_str());
        if (renameStatus != 0)
        {
            wcout << L"Failed to rename file " << originalPath << file << L'\n';
        }
        else
        {
            wcout << L"Renamed file " << originalPath << file << L" To " << newFullName << L'\n';
            ++startCount;
        }
    }
    wcout << L"\nFinished" << endl;

    return 0;
}