#include <iostream>
#include "JSONParser.h"

int main()
{
    try {
        JSONParser jp;
        jp.parseFile("test.json.txt");
        jp.printFile();
        std::cout << std::endl;
        jp.searchKey("id");
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