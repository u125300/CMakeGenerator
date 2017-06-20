#include <iostream>
#include <vector>

#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>

#undef BOOST_NO_CXX11_SCOPED_ENUMS

using namespace std;
using namespace boost::filesystem;

vector<path> files;
vector<string> dirs;
vector<string> executable;

void search(string pth)
{
    path p(pth);
    for(auto i = directory_iterator(p); i != directory_iterator(); i++)
    {
        if(!is_directory(i->path()))
        {
            string ext = i->path().extension().string();
            if(ext == ".c" || ext == ".cpp" || ext == ".cxx" || ext == ".h" || ext == ".hpp")
            {
                files.push_back(i->path());
            }
        }
        else
        {
            search(i->path().string());
        }

    }
}

vector<string> split(string source, string splitter)
{
    typedef vector<string> split_vector;
    split_vector SplitVec;

    boost::split(SplitVec, source, boost::is_any_of(splitter), boost::token_compress_on);

    return SplitVec;
}

bool isExist(string candidate)
{
    bool exist = false;
    for(string p : dirs)
    {
        if(p == candidate) exist = true;
    }
    return exist;
}

void sort(string pth)
{
    for(path p : files)
    {
        string parent = p.parent_path().string();
        string b_pth = parent.substr(pth.length(), parent.length());

        if(b_pth.length() > 0)
        {
            executable.push_back(b_pth.substr(1) + '/' + p.filename().string());

            string dir = (basic_string<char, char_traits<char>, allocator<char>> &&) split(b_pth, "/").at(1);
            if (!isExist(dir)) dirs.push_back(dir);
        } else executable.push_back(p.filename().string());
    }
}

void createFile(string pth, string project)
{
   if(pth.back() != '/')
   {
       pth += '/';
   }

    ofstream outfile (pth + "CMakeLists.txt.gen");
    outfile << "cmake_minimum_required(VERSION 3.7)" << endl;
    outfile << "project(" + project + ")" << endl;

    for(string dir : dirs)
    {
        outfile << "include_directories(" << dir << ")" << endl;
    }

    outfile << "add_executable(" << project;
    for(string file : executable)
    {
        outfile << " " << file;
    }

    outfile << ")";
    outfile.close();
}

int main(int argc, char *argv[]) {
    if(argc < 2)
    {
        cout << "Usage " << argv[0] << " <src_path>" << endl;
        return 0;
    }

    search(argv[1]);
    sort(argv[1]);

    vector<string> projectList = split(argv[1], "/");
    string project = (basic_string<char, char_traits<char>, allocator<char>> &&) projectList.at(projectList.size() - 1);

    createFile(argv[1], project);

    cout << "cmake_minimum_required(VERSION 3.7)" << endl;
    cout << "project(" + project + ")" << endl;

    for(string dir : dirs)
    {
        cout << "include_directories(" << dir << ")" << endl;
    }

    cout << "add_executable(" << project;
    for(string file : executable)
    {
        cout << " " << file;
    }

    cout << ")";
    return 0;
}