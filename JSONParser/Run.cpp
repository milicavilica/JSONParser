#include "Run.h"
#include <Windows.h>

void Run::runValidate(const JSONParser& jp, int& reprintMenu)
{
	std::cout << "Enter file name: ";
	char fileName[1024];
	std::cin.getline(fileName, 1024);
	try {
		jp.validateFile(fileName);
	}
	catch (std::exception& exc)
	{
		std::cout << std::endl << exc.what() << std::endl;
	}
	std::cout << std::endl << std::endl;
	reprintMenu++;
}

void Run::runOpenFile(JSONParser& jp, int& reprintMenu)
{
	
	std::cout << "Enter file name: ";
	char fileName[1024];
	std::cin.getline(fileName, 1024);
	try
	{
		jp.parseFile(fileName);
	}
	catch (std::exception& exc)
	{
		std::cout << std::endl << exc.what() << std::endl;
	}
	std::cout << std::endl;
	reprintMenu++;
}

void Run::runSearchKey(const JSONParser& jp, int& reprintMenu)
{
	char keyBuff[1024]{};
	std::cout << "Enter a key: ";
	std::cin.getline(keyBuff, 1024);
	jp.searchKey(keyBuff);
	std::cout << std::endl;
	reprintMenu++;
}

void Run::runSet(JSONParser& jp, int& reprintMenu)
{
	char pathBuff[1024]{};
	std::cout << "Enter a path: ";
	std::cin.getline(pathBuff, 1024);
	char value[1024]{};
	std::cout << "Enter a value: ";
	std::cin.getline(value, 1024);
	try
	{
		MyString path(pathBuff);
		jp.set(path, value);
	}
	catch (std::exception& exc)
	{
		std::cout << std::endl << exc.what() << std::endl;
	}
	std::cout << std::endl;
	reprintMenu++;
}

void Run::runCreate(JSONParser& jp, int& reprintMenu)
{
	char pathBuff[1024]{};
	std::cout << "Enter a path: ";
	std::cin.getline(pathBuff, 1024);
	char value[1024]{};
	std::cout << "Enter a value: ";
	std::cin.getline(value, 1024);
	try
	{
		MyString path(pathBuff);
		jp.create(path, value);
	}
	catch (std::exception& exc)
	{
		std::cout << std::endl << exc.what() << std::endl;
	}
	std::cout << std::endl;
	reprintMenu++;
}

void Run::runDelete(JSONParser& jp, int& reprintMenu)
{
	char pathBuff[1024]{};
	std::cout << "Enter a path: ";
	std::cin.getline(pathBuff, 1024);
	try
	{
		MyString path(pathBuff);
		jp.deleteValue(path);
	}
	catch (std::exception& exc)
	{
		std::cout << std::endl << exc.what() << std::endl;
	}
	std::cout << std::endl;
	reprintMenu++;
}

void Run::runMove(JSONParser& jp, int& reprintMenu)
{
	char pathBuff[1024]{};
	std::cout << "Enter a 'from' path: ";
	std::cin.getline(pathBuff, 1024);
	char toBuff[1024]{};
	std::cout << "Enter a 'to' path: ";
	std::cin.getline(toBuff, 1024);
	try
	{
		MyString path(pathBuff);
		MyString to(toBuff);
		jp.move(path, to);
	}
	catch (std::exception& exc)
	{
		std::cout << std::endl << exc.what() << std::endl;
	}
	std::cout << std::endl;
	reprintMenu++;
}

void Run::runSave(const JSONParser& jp, int& reprintMenu)
{
	std::cout << "Do you want to save the whole object? yes/no ";
	char answear[1024]{};
	std::cin.getline(answear, 1024);
	int yesCmp = strcmp(answear, "yes");
	int YesCmp = strcmp(answear, "Yes");
	int noCmp = strcmp(answear, "no");
	int NoCmp = strcmp(answear, "No");
	while (yesCmp != 0 && noCmp != 0 && NoCmp != 0 && YesCmp != 0)
	{
		std::cout << "Enter yes or no: ";
		answear[0] = '\0';
		std::cin.getline(answear, 1024);
		yesCmp = strcmp(answear, "yes");
		YesCmp = strcmp(answear, "Yes");
		noCmp = strcmp(answear, "no");
		NoCmp = strcmp(answear, "No");
	}
	if (!yesCmp || !YesCmp)
	{
		try
		{
			jp.save();
		}
		catch (std::exception& exc)
		{
			std::cout << std::endl << exc.what() << std::endl;
		}
	}
	else if (!noCmp || !NoCmp)
	{
		std::cout << "Enter path: ";
		char pathBuff[1024]{};
		std::cin.getline(pathBuff, 1024);
		try
		{
			MyString path(pathBuff);
			jp.save(path);
		}
		catch (std::exception& exc)
		{
			std::cout << std::endl << exc.what() << std::endl;
		}
	}
	std::cout << "Object successfully saved in " << jp.getSourceFile() << std::endl;
	std::cout << std::endl;
	reprintMenu++;
}

void Run::runSaveAs(const JSONParser& jp, int& reprintMenu)
{
	char fileName[1024]{};
	std::cout << "Enter file name to save: ";
	std::cin.getline(fileName, 1024);
	std::cout << "Do you want to save the whole object? yes/no ";
	char answear[1024]{};
	std::cin.getline(answear, 1024);
	int yesCmp = strcmp(answear, "yes");
	int YesCmp = strcmp(answear, "Yes");
	int noCmp = strcmp(answear, "no");
	int NoCmp = strcmp(answear, "No");
	while (yesCmp != 0 && noCmp != 0 && NoCmp != 0 && YesCmp != 0)
	{
		std::cout << "Enter yes or no: ";
		answear[0] = '\0';
		std::cin.getline(answear, 1024);
		yesCmp = strcmp(answear, "yes");
		YesCmp = strcmp(answear, "Yes");
		noCmp = strcmp(answear, "no");
		NoCmp = strcmp(answear, "No");
	}
	if (!yesCmp || !YesCmp)
	{
		try
		{
			jp.saveas(fileName);
		}
		catch (std::exception& exc)
		{
			std::cout << std::endl << exc.what() << std::endl;
		}
	}
	else if (!noCmp || !NoCmp)
	{
		std::cout << "Enter path: ";
		char pathBuff[1024]{};
		std::cin.getline(pathBuff, 1024);
		try
		{
			MyString path(pathBuff);
			jp.saveas(fileName, path);
		}
		catch (std::exception& exc)
		{
			std::cout << std::endl << exc.what() << std::endl;
		}
	}
	std::cout << "Object successfully saved in " << fileName << std::endl;
	std::cout << std::endl;
	reprintMenu++;
}

void Run::run()
{
	JSONParser jp;
	JSONParser::printMenu();
	bool run = true;
	int reprintMenu = 0;
	while (run)
	{
		if (reprintMenu == 5)
		{
			Sleep(2000);
			JSONParser::printMenu();
			reprintMenu = 0;
		}
		std::cout << "Enter your choice: ";
		char buffer[1024]{};
		std::cin.getline(buffer, 1024);
		size_t choice = toNum(buffer);
		while (choice < 1 || choice > 11) {
			std::cout << "Enter a nubmer between 1 and 11: ";
			buffer[0] = '\0';
			std::cin.getline(buffer, 1024);
			choice = toNum(buffer);
			std::cout << std::endl;
		}

		switch (choice)
		{
		case 1:
		{
			runValidate(jp, reprintMenu);
			break;
		}
		case 2:
		{
			if (jp.exists())
			{
				std::cout << "File alredy opened" << std::endl;
				break;
			}
			runOpenFile(jp, reprintMenu);
			break;
		}
		case 3:
		{
			if (!jp.exists())
			{
				std::cout << "No file has been parsed yet" << std::endl;
				break;
			}
			jp.printFile();
			std::cout << std::endl;
			reprintMenu++;
			break;
		}
		case 4:
		{
			if (!jp.exists())
			{
				std::cout << "No file has been parsed yet" << std::endl;
				break;
			}
			runSearchKey(jp, reprintMenu);
			break;
		}
		case 5:
		{
			if (!jp.exists())
			{
				std::cout << "No file has been parsed yet" << std::endl;
				break;
			}
			runSet(jp, reprintMenu);
			break;
		}
		case 6:
		{
			if (!jp.exists())
			{
				std::cout << "No file has been parsed yet" << std::endl;
				break;
			}
			runCreate(jp, reprintMenu);
			break;
		}
		case 7:
		{
			if (!jp.exists())
			{
				std::cout << "No file has been parsed yet" << std::endl;
				break;
			}
			runDelete(jp, reprintMenu);
			break;
		}
		case 8:
		{
			if (!jp.exists())
			{
				std::cout << "No file has been parsed yet" << std::endl;
				break;
			}
			runMove(jp, reprintMenu);
			break;
		}
		case 9:
		{
			if (!jp.exists())
			{
				std::cout << "No file has been parsed yet" << std::endl;
				break;
			}
			runSave(jp, reprintMenu);
			break;
		}
		case 10:
		{
			if (!jp.exists())
			{
				std::cout << "No file has been parsed yet" << std::endl;
				break;
			}
			runSaveAs(jp, reprintMenu);
			break;
		}
		case 11:
		{
			std::cout << "You exited the program!";
			run = false;
		}
		default:
			break;
		}
	}
}
