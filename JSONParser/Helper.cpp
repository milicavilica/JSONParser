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
bool stringBeginsWith(const MyString& string, const MyString& piece)
{
	if (piece.length() - 1 > string.length())
		return false;
	MyString substr = string.substr(0, piece.length() - 1);
	return substr == piece.substr(0, piece.length() - 1);;
}