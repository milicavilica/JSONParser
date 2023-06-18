#include "Helper.h"
#include <iostream>

void removeQuotesFront(char* str)
{
	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++)
	{
		str[i] = str[i + 1];
	}
}
void removeQuotesBack(char* str)
{
	str[strlen(str) - 1] = '\0';
}
void removeQuotes(char* str)
{
	while (str[0] == '"' || str[0] == ' ' || str[0] == '\t')
		removeQuotesFront(str);
	size_t len = strlen(str);
	while (str[len - 1] == '"' || str[len - 1] == ' ' || str[len - 1] == '\t') {
		removeQuotesBack(str);
		len--;
	}

}
bool isThereAColon(std::istream& ifs)
{
	

	char colon = ifs.get();
	if (ifs.eof())
		return false;
	while (true)
	{
		if (colon == ':') {
			
			//ifs.seekg(-1, std::ios::cur);
			return true;
		}
		if (colon == '\"' || colon == ',')
			return false;
		if (colon == ']' || colon == '}')
		{
			ifs.seekg(-1, std::ios::cur);
			return false;
		}
		if (colon == ' ' || colon == '\t' || colon == '\n')
			colon = ifs.get();
	}
}
bool stringBeginsWith(const MyString& string, const MyString& piece)
{
	if (piece.length() - 1 > string.length())
		return false;
	MyString substr = string.substr(0, piece.length() - 1);
	return substr == piece.substr(0, piece.length() - 1);;
}

bool cutPathAndCheckKey(MyString& path, const MyString& key)
{
	size_t j = 0;
	while (path[j] != '/' && path[j] != '\0')
		j++;
	if (key != path.substr(0, j))
		return false;
	path = path.substr(j + 1, path.length() - j - 1);
	return true;
}
void cutPath(MyString& path)
{
	size_t j = 0;
	while (path[j] != '/' && path[j] != '\0')
		j++;
	path = path.substr(j + 1, path.length() - j - 1);
}
size_t countSlashes(const MyString& path)
{
	size_t slashCount = 0;
	size_t i = 0;
	while (path[i] != '\0')
	{
		if (path[i++] == '/')
			slashCount++;
	}
	return slashCount;
}
size_t toNum(const char* str)
{
	int i = 0;
	size_t num = 0;
	while (str[i] != '\0')
	{
		num += (str[i++] - '0');
		num *= 10;
	}
	return num/10;
}