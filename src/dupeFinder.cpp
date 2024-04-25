// g++ dupeFinder.cpp -municode -o dupefinder -O2 -Wall -Wextra -s

#include <iostream>
#include <string.h>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

int wmain([[maybe_unused]] int argc, [[maybe_unused]] wchar_t **argv)
{
    using namespace std;
    namespace fs = filesystem;

    ios_base::sync_with_stdio(false);

    fs::path dir = fs::current_path();
    cout << "Finding duplicates in " << dir << endl;

    unordered_map<size_t, vector<fs::path>> hash_to_paths;

    unsigned count = 0;
    for (const fs::directory_entry &entry : fs::directory_iterator(dir))
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