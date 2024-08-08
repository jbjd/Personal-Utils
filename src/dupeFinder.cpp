// g++ dupeFinder.cpp -municode -o dupefinder -O2 -Wall -Wextra -s

#include <iostream>
#include <string.h>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

void collectHashesInFolder(
    std::filesystem::path path,
    std::unordered_map<size_t, std::vector<std::filesystem::path>> &hash_to_paths,
    unsigned &count)
{
    using namespace std;
    namespace fs = filesystem;

    for (const fs::directory_entry &entry : fs::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            continue;
        }
        // get hash of file
        ifstream is(entry.path(), ios::in | ios::binary | ios::ate);
        if (!is)
        {
            cerr << "Failed to open file " << entry.path() << endl;
        }

        streampos size = is.tellg();
        string file_content(size, '\0');
        is.seekg(0);
        is.read(&file_content[0], size);
        is.close();

        hash_to_paths[std::hash<std::string>{}(file_content)].push_back(entry.path());

        if (++count % 100 == 0)
        {
            wcout << "Loaded " << count << " files" << endl;
        }
    }
}

int wmain(int argc, wchar_t **argv)
{
    using namespace std;
    namespace fs = filesystem;

    ios_base::sync_with_stdio(false);

    fs::path dir = fs::current_path();
    wcout << "Finding duplicates in " << dir;
    for (int i = 1; i < argc; ++i)
    {
        wcout << " and \"" << argv[i] << "\"";
    }
    wcout << '\n'
          << endl;

    unordered_map<size_t, vector<fs::path>> hash_to_paths;

    unsigned count = 0;
    collectHashesInFolder(dir, hash_to_paths, count);
    for (int i = 1; i < argc; ++i)
    {
        fs::path pathToAdd = argv[i];
        collectHashesInFolder(pathToAdd, hash_to_paths, count);
    }

    for (const auto &dupe_entry : hash_to_paths)
    {
        vector<fs::path> dupe_list = dupe_entry.second;
        if (dupe_list.size() > 1)
        {
            wcout << L"Possible duplicates:";
            for (fs::path path : dupe_list)
            {
                wcout << ' ' << path;
            }
            wcout << '\n';
        }
    }

    wcout << flush;
    return 0;
}