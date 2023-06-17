#include "JSONSimpleValue.h"

JSONSimpleValue::JSONSimpleValue(const MyString& key, const MyString& val) : JSON(key)
{
	value = val;
}


const MyString& JSONSimpleValue::getValue() const
{
	return value;
}

void JSONSimpleValue::print(unsigned tabsCnt) const
{
	for (size_t i = 0; i < tabsCnt; i++)
	{
		std::cout << '\t';
	}
	if (key.length() != 0)
		std::cout << '"' <<  getKey()  << '"' << ":";
	std::cout << '"' << getValue() << '"';
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
void JSONSimpleValue::printValue() const
{
	std::cout << '"' << value << '"';
}
char JSONSimpleValue::getType() const
{
	return SIMPLE_DATA;
}
bool JSONSimpleValue::set(MyString& path, const char* newValue, bool& succes) 
{
	return path == key;
}
bool JSONSimpleValue::set(MyString& path, const JSON* element, bool& succes)
{
	return path == key;
}


