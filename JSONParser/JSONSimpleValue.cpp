#include "JSONSimpleValue.h"

JSONSimpleValue::JSONSimpleValue(const MyString& key, const MyString& val) : JSON(key)
{
	value = val;
}


const MyString& JSONSimpleValue::getValue() const
{
	return value;
}

void JSONSimpleValue::print(std::ostream& os, unsigned tabsCnt) const
{
	//printing tabs
	for (size_t i = 0; i < tabsCnt; i++)
	{
		os << '\t';
	}
	if (key.length() != 0)
		os << '"' <<  getKey()  << '"' << ":";
	printValue(os);
}
void JSONSimpleValue::save(MyString& path, std::ostream& ofs, bool& success) const
{
	if (path == key) {
		success = true;
		print(ofs);
		ofs << std::endl;
	}
}
JSON* JSONSimpleValue::findElement(MyString& path)
{
	if (path == key)
		return this;
	return nullptr;
}

JSON* JSONSimpleValue::clone() const
{
	return new JSONSimpleValue(*this);
}
void JSONSimpleValue::searchKey(const MyString& _key) const
{
	if ((_key[_key.length() - 1] == '*' && stringBeginsWith(this->getKey(), _key)) || this->key == _key)
	{
		std::cout << '"' << value << '"';
		std::cout << ',' << std::endl;
		
	}
}
bool JSONSimpleValue::deleteValue(MyString& path)
{
	return path == key;
}
void JSONSimpleValue::create(MyString& path, const char* value)
{}
void JSONSimpleValue::create(MyString& path, const JSON* element)
{}
void JSONSimpleValue::printValue(std::ostream& os) const
{
	os << '"' << value << '"';
}
char JSONSimpleValue::getType() const
{
	return SIMPLE_DATA;
}
bool JSONSimpleValue::set(MyString& path, const char* newValue, bool& success) 
{
	return path == key;
}
bool JSONSimpleValue::set(MyString& path, const JSON* element, bool& success)
{
	return path == key;
}


