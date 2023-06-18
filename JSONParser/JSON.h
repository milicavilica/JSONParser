#pragma once
#include "MyString.h"
#include "Vector.hpp"
#include "Helper.h"

#include <iostream>


const char OBJECT = '{';
const char ARRAY = '[';
const char SIMPLE_DATA = '\"';

class JSON
{
protected:
	MyString key;
public:
	JSON() = default;
	JSON(const MyString& _key);
	void setKey(const MyString& str);
	const MyString& getKey() const;
	virtual char getType() const = 0;

	virtual void searchKey(const MyString& _key) const = 0;
	virtual void print(std::ostream& os, unsigned tabsCnt = 0) const = 0;//unsigned tabs count = 0 idea by Nasko
	virtual void printValue(std::ostream& os) const = 0;

	virtual bool set(MyString& path, const char* newValue, bool& success) = 0;
	virtual bool set(MyString& path, const JSON* element, bool& success) = 0;

	virtual bool deleteValue(MyString& path) = 0;
	virtual void create(MyString& path, const char* value) = 0;
	virtual void create(MyString& path, const JSON* element) = 0;

	virtual void save(MyString& path, std::ostream& ofs, bool& success) const = 0;

	virtual JSON* findElement(MyString& path) = 0;
	virtual JSON* clone() const = 0;

	virtual ~JSON() = default;
};

