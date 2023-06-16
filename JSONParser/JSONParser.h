#pragma once
#include "JSON.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONSimpleValue.h"
#include <iostream>
#include "JSONFactory.h"
#include "Helper.h"

class JSONParser
{
	JSON* data = nullptr;
	void free();
	void copyFrom(const JSONParser& other);
	void moveFrom(JSONParser&& other);

	bool validateQuotes(std::ifstream& ifs) const;
	bool validateBrack(std::ifstream& ifs, const char ch) const;
	void validateLogic(std::ifstream& ifs, char ch) const;
public:
	JSONParser() = default;
	JSONParser(const char* fileName);
	JSONParser(const JSONParser& other);
	JSONParser(JSONParser&& other);

	JSONParser& operator=(const JSONParser& other);
	JSONParser& operator=(JSONParser&& other);

	void parseFile(const char* fileName);
	void printFile() const;
	void validateFile(std::ifstream& ifs) const;
	void validateFile(const char* fileName) const;
	void searchKey(const MyString& _key) const;
	void set(MyString& path, const char* value);
	void deleteValue(MyString& path);
	void create(MyString& path, const char* value);
	

	~JSONParser();
};