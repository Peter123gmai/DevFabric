#include <iostream>
#include <vector>

int main(int argc, char*argv[])
{
    std::vector <std::string> sargv = {};
    for (int i = 0; i<argc;i++){sargv.push_back(argv[i]);}
    return 0;
}