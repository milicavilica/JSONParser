#include <iostream>
#include "JSONParser.h"

int main()
{
    try {
        JSONParser jp;
        jp.parseFile("test.json.txt");
        jp.printFile();
        std::cout << std::endl << std::endl;
        //
       
        char path[1024]{};
        char value[1024]{};
        std::cin.getline(path, 1024);
        std::cin.getline(value, 1024);
        MyString p(std::move(path));
        jp.set(p, value);
        jp.printFile();
    }
    catch (std::invalid_argument& exc)
    {
        std::cout << exc.what();
    }
    catch (std::exception& exc)
    {
        std::cout << exc.what();
    }
    catch (...)
    {
        std::cout << "unknown exception";
    }
    

}