#include <iostream>
#include "JSONParser.h"

int main()
{
    try {
        JSONParser jp;
        jp.parseFile("test.json.txt");
        jp.printFile();
        std::cout << std::endl;
        MyString st = "age";
        jp.create(st, "[\"1\", \"2\"]");
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