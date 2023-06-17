#pragma once
#include "MyString.h"

void removeQuotes(char* str);
void removeQuotesFront(char* str);
void removeQuotesBack(char* str);
bool stringBeginsWith(const MyString& string, const MyString& piece);
bool cutPathAndCheckKey(MyString& path, const MyString& key);
void cutPath(MyString& path);
bool isThereAColon(std::istream& ifs);
size_t countSlashes(const MyString& path);