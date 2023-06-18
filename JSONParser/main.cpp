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
       
      /*  char path[1024]{};
        char value[1024]{};
        std::cin.getline(path, 1024);
        std::cin.getline(value, 1024);
        MyString p(std::move(path));
        jp.set(p, value);
        char path1[1024]{};
        char value1[1024]{};
        std::cin.getline(path1, 1024);
        std::cin.getline(value1, 1024);
        MyString p1(std::move(path1));
        jp.set(p1, value1);
        jp.printFile();*/
        MyString ppp = "offices";
        jp.saveas("test2.json.txt", ppp);
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