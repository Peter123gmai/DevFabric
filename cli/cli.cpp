#include <iostream>
#include <vector>
#include <string>
//#include <map>

struct systeminfo 
{
    const std::string ver = "1.20";
};

int main(int argc, char*argv[])
{
    std::vector<std::string> argvs = {};
    systeminfo syif;
    
    for (int i = 0; i<argc; i++){argvs.push_back(argv[i]);}
    if (argvs.size() < 2)
    {
        std::cout << "Error: Missing argument" << std::endl;
        exit(1);
    }

    if (argvs[1] == "-v" || argvs[1] == "--v" || argvs[1] == "--version" || argvs[1] == "-version"){std::cout << syif.ver << std::endl;}
    return 0;
}