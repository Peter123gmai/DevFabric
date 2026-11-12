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
    systeminfo InfoCli;
    
    // loading char*argv to vector argvs
    for (int i = 0; i<argc; i++){argvs.push_back(argv[i]);}

    //error handling
    if (argvs.size() < 2)
    {
        std::cout << "Error: Missing argument" << std::endl;
        exit(1);
    }
    if (argvs[1] == "-" || argvs[1] == "--")
    {
        std::cout << "Error: missing argument after `-` or `--`" << std::endl;
        std::cout << "Use the help command for more information on how to use it" << std::endl;
        exit(1);
    }

    if (argvs[1] == "-v" || argvs[1] == "--v" || argvs[1] == "--version" || argvs[1] == "-version"){std::cout << InfoCli.ver << std::endl;} // version check

    
    return 0;
}