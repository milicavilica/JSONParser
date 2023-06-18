#include "JSONParser.h"
#include <fstream>
#include <sstream>



JSONParser::JSONParser(const char* fileName)//delete it
{
	parseFile(fileName);
}
void JSONParser::free()
{
	delete data;
}
void JSONParser::copyFrom(const JSONParser& other)
{
	data = other.data->clone();
	sourceFile = other.sourceFile;
}
void JSONParser::moveFrom(JSONParser&& other)
{
	data = other.data;
	other.data = nullptr;
	sourceFile = std::move(other.sourceFile);
}
JSONParser::JSONParser(const JSONParser& other)
{
	copyFrom(other);
}
JSONParser::JSONParser(JSONParser&& other)
{
	moveFrom(std::move(other));
}

JSONParser& JSONParser::operator=(const JSONParser& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
JSONParser& JSONParser::operator=(JSONParser&& other)
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

void JSONParser::parseFile(const char* fileName)
{
	std::ifstream ifs(fileName, std::ios::in);
	if (!ifs.is_open())
		throw std::exception("File couldn't be opened");

	sourceFile = fileName;
	try {
		validateFile(ifs);
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return;
	}
	

	ifs.clear();
	ifs.seekg(0, std::ios::beg);
	char c;
	c = ifs.peek();
	
	data = factory(c, ifs);///

	ifs.close();

	std::cout << "and parsed" << std::endl;//print for menu
}
bool JSONParser::validateQuotes(std::istream& ifs) const
{
	size_t currentPos = ifs.tellg();
	int counter = 0;
	char c;
	while (true)
	{
		c = ifs.get();
		if (ifs.eof())
			break;
		if (c == '\"')
			counter++;
	}
	ifs.clear();
	ifs.seekg(currentPos);
	return counter % 2 == 0;
}
bool JSONParser::validateBrack(std::istream& ifs, const char ch) const
{
	size_t currentPos = ifs.tellg();
	int counter = 0;
	char c;
	if (ch == OBJECT)
	{
		while (!ifs.eof())
		{
			c = ifs.get();
			if (c == '{')
				counter++;
			else if (c == '}')
				counter--;
		}
		
	}
	else
	{
		while (!ifs.eof())
		{
			c = ifs.get();
			if (c == '[')
				counter++;
			else if (c == ']')
				counter--;
		}
	}
	ifs.clear();
	ifs.seekg(currentPos);
	return counter == 0;
}
void JSONParser::validateLogic(std::istream& ifs, char c) const
{
	size_t currentPos = ifs.tellg();
	switch (c)
	{
	case SIMPLE_DATA:
	{
		c = ifs.get();
		while (c != '"') {
			if (c == ':' || c == '{' || c == '}' || c == ']' || c == '[' || c == ',' || c == '\n')
				throw std::exception("Incorrect symbol in key or value!");
			c = ifs.get();
		}
		currentPos = ifs.tellg();
		break;
	}
	case ARRAY:
	{
		c = ifs.get();
		while (c == ' ' || c == '\t' || c == ',' || c == '\n')
			c = ifs.get();

		while (c != ']')
		{
			if (c == SIMPLE_DATA)
				validateLogic(ifs, '"');
			if (c == ARRAY)
				validateLogic(ifs, '[');
			if (c == OBJECT)
				validateLogic(ifs, '{');

			if (c == ':')
				throw std::exception("Cannot have key-value pairs in an array");

			c = ifs.get();
			if (c == ']')
				break;
		}
		currentPos = ifs.tellg();
		
		break;
	}
	case OBJECT:
	{
		c = ifs.get();
		while (c == ' ' || c == '\t' || c == ',' || c == '\n')
			c = ifs.get();

		while (c != '}')
		{
			if (c == SIMPLE_DATA) {

				validateLogic(ifs, '"');
				char ch = ifs.get();

				while (ch == ' ' || ch == '\t')
					ch = ifs.get();
				if(ch != ':')
					throw std::exception("Can't have a value without a key in an object!");
				ch = ifs.get();

				while (ch == ' ' || ch == '\t')
					ch = ifs.get();

				if (ch == '"')
					validateLogic(ifs, '"');
				else if(ch == '[')
					validateLogic(ifs, '[');
				ch = ifs.get();
				while (ch == ' ' || ch == '\t')
					ch = ifs.get();
				if (ch == '}')
					break;
				if(ch != ',' && ch != '\n')
					throw std::exception("Wrong kvp syntax in an object!");


			}
			if (c == ARRAY) {
				ifs.seekg(-1, std::ios::cur);
				char ch = ifs.get();
				while (ch == ' ' || ch == '\t')
					ch = ifs.get();
				if (ch != ':')
					throw std::exception("Can't have an array whitout a key in an object!");
				ch = ifs.get();

				while (ch == ' ' || ch == '\t')
					ch = ifs.get();
				ifs.ignore();
				validateLogic(ifs, '[');
			}
			if (c == OBJECT)
				validateLogic(ifs, '{');

			c = ifs.get();

		}
		currentPos = ifs.tellg();
		break;
	}
	default:
		throw std::exception("Wrong file format!");
		break;
	}
	ifs.clear();
	ifs.seekg(currentPos);
}
void JSONParser::validateFile(std::istream& ifs) const
{
	if (!validateQuotes(ifs))
		throw std::exception("incorrect placemen of quotes in file!");
	if (!validateBrack(ifs, ARRAY))
		throw std::exception("incorrect placemen of square brackets in file!");
	if (!validateBrack(ifs, OBJECT))
		throw std::exception("incorrect placemen of curly brackets in file!");
	char c;
	c = ifs.get();
	validateLogic(ifs, c);
	std::cout << "File successfully validated ";
}
void JSONParser::validateFile(const char* fileName) const
{
	std::ifstream ifs(fileName);
	if (!ifs.is_open())
		throw std::exception("File could't be opened!");
	validateFile(ifs);
	ifs.close();
}
void JSONParser::printFile() const
{
	int withKey = 1;
	data->print(std::cout, withKey);
}
void JSONParser::searchKey(const MyString& _key) const
{
	
	std::cout << '"' << _key << "\": " << std::endl;
	std::cout << '[' << std::endl <<std::endl;
	bool success = false;
	data->searchKey(_key, success);
	if (!success)
		std::cout << "Incorrect path!" << std:: endl;
	std::cout << std::endl;
	std::cout << ']';
}
void JSONParser::set(MyString& path, const char* val)
{
	std::stringstream ss(val);
	validateFile(ss);
	bool success = false;
	data->set(path, val, success);
	if (!success)
		throw std::exception("Incorrect path!");
	std::cout << std::endl << "Value successfully set" << std::endl;
}
void JSONParser::deleteValue(MyString& path)
{
	bool success = false;
	data->deleteValue(path, success);
	if (!success)
		throw std::exception("Incorrect path!");
}
void JSONParser::create(MyString& path, const char* newValue)
{
	std::stringstream ss(newValue);
	validateFile(ss);
	bool success = false;
	data->create(path, newValue, success);
	if (!success)
		throw std::exception("Incorrect path!");

}
void JSONParser::move(MyString& from, MyString& to)
{
	MyString fromKey = from;
	while (countSlashes(fromKey) > 0)
	{
		cutPath(fromKey);
	}
	MyString toKey = to;
	while (countSlashes(toKey) > 0)
	{
		cutPath(toKey);
	}
	JSON* element = data->findElement(from);
	if(element == nullptr)
		throw std::exception("Incorrect path!");
	element->setKey(toKey);
	MyString empty("EMPTY");
	JSON* nullEl = new JSONSimpleValue(fromKey, empty);
	bool set1Success = false;
	data->set(from, nullEl,  set1Success);
	if (!set1Success)
		throw std::exception("Incorrect path!");
	bool set2Success = false;
	data->set(to, element, set2Success);
	if (!set2Success)
		throw std::exception("Incorrect path!");
	std::cout << "Object successfully moved!" << std::endl;
}

void JSONParser::save(MyString& path) const
{
	std::ofstream ofs(sourceFile.c_str(), std::ios::out | std::ios::trunc);
	if(!ofs.is_open())
		throw std::exception("couldn't open file!");

	bool success = false;
	data->save(path, ofs, success);
	if (!success)
		throw std::exception("incorrect path!");

	ofs.close();
}
void JSONParser::save() const
{
	std::ofstream ofs(sourceFile.c_str(), std::ios::out | std::ios::trunc);
	if (!ofs.is_open())
		throw std::exception("Couldn't open file!");
	int withKey = 1;
	data->print(ofs, withKey);
	ofs.close();
}
const MyString& JSONParser::getSourceFile() const
{
	return sourceFile;
}
void JSONParser::saveas(const char* filename, MyString& path) const
{
	std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
	if (!ofs.is_open())
		throw std::exception("couldn't open file!");

	bool success = false;
	data->save(path, ofs, success);
	if (!success)
		throw std::exception("incorrect path!");

	ofs.close();
}
void JSONParser::saveas(const char* filename) const
{
	std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
	if (!ofs.is_open())
		throw std::exception("couldn't open file!");
	int withKey = 1;
	data->print(ofs ,withKey);
	ofs.close();
}

JSONParser::~JSONParser()
{
	free();
}

bool JSONParser::exists() const
{
	return data != nullptr;
}

void JSONParser::printMenu()
{
	std::cout << "Menu:" << std::endl;
	std::cout << line << std::endl;
	std::cout << "1. Validate JSON file" <<  std::endl;
	std::cout << "2. Open file" << std::endl;
	std::cout << "3. Print file" << std::endl;
	std::cout << "4. Search" << std::endl;
	std::cout << "5. Set value" << std::endl;
	std::cout << "6. Create value" << std::endl;
	std::cout << "7. Delete value" << std::endl;
	std::cout << "8. Move value" << std::endl;
	std::cout << "9. Save" << std::endl;
	std::cout << "10. Save as" << std::endl;
	std::cout << "11. Exit program" << std::endl;
	std::cout << line << std::endl;

}


