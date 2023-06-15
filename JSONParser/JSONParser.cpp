#include "JSONParser.h"
#include <fstream>
#include <sstream>



JSONParser::JSONParser(const char* fileName)
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
}
void JSONParser::moveFrom(JSONParser&& other)
{
	data = other.data;
	other.data = nullptr;
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
		throw std::exception("file couldn't be opened");

	try {
		validateFile(ifs);
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
		return;
	}

	ifs.clear();
	ifs.seekg(0, std::ios::beg);
	char c;
	c = ifs.peek();
	
	data = factory(c, ifs);///

	ifs.close();

}
bool JSONParser::validateQuotes(std::ifstream& ifs) const
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
bool JSONParser::validateBrack(std::ifstream& ifs, const char ch) const
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
void JSONParser::validateLogic(std::ifstream& ifs, char c) const
{
	size_t currentPos = ifs.tellg();
	switch (c)
	{
	case SIMPLE_DATA:
	{
		c = ifs.get();
		while (c != '"') {
			if (c == ':' || c == '{' || c == '}' || c == ']' || c == '[' || c == ',' || c == '\n')
				throw std::exception("incorrect symbol in key or value!");
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
				throw std::exception("cannot have key-value pairs in an array");

			c = ifs.get();
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
					throw std::exception("can't have a value without a key in an object!");
				ch = ifs.get();

				while (ch == ' ' || ch == '\t')
					ch = ifs.get();

				if (ch == '"')
					validateLogic(ifs, '"');
				else if(ch == '[')
					validateLogic(ifs, '[');
				ch = ifs.get();
				if(ch != ',' && ch != '\n')
					throw std::exception("wrong kvp syntax in an object!");


			}
			if (c == ARRAY) {
				ifs.seekg(-1, std::ios::cur);
				char ch = ifs.get();
				while (ch == ' ' || ch == '\t')
					ch = ifs.get();
				if (ch != ':')
					throw std::exception("can't have an array whitout a key in an object!");
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
		break;
	}
	ifs.clear();
	ifs.seekg(currentPos);
}
void JSONParser::validateFile(std::ifstream& ifs) const
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
	std::cout << "file succesfully validated!" << std::endl;
}
void JSONParser::validateFile(const char* fileName) const
{
	std::ifstream ifs(fileName);
	if (!ifs.is_open())
		throw std::exception("file could't be opened!");
	validateFile(ifs);
	ifs.close();
}
void JSONParser::printFile() const
{
	data->print();
}
void JSONParser::searchKey(const MyString& _key) const
{
	
	std::cout << '"' << _key << "\": " << std::endl;
	std::cout << '[' << std::endl;
	data->searchKey(_key);
	std::cout << std::endl;
	std::cout << ']';
}

JSONParser::~JSONParser()
{
	free();
}
