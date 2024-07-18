

#include <tdl/Utils/SubShell.hpp>

#include <iostream>


int main()
{
    tdl::subShell shell;
    shell.openSubShell();
    shell.writeOnSubShell("ls\n");
    while (1) {
        std::string str = shell.readOnSubShell();
        if (str == "")
            continue;
        std::cout << str << std::endl;
    }
}