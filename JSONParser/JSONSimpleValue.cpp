#include "JSONSimpleValue.h"

JSONSimpleValue::JSONSimpleValue(const MyString& key, const MyString& val) : JSON(key)
{
	value = val;
}


const MyString& JSONSimpleValue::getValue() const
{
	return value;
}

void JSONSimpleValue::print(std::ostream& os, int& withKey, unsigned tabsCnt) const
{
	//printing tabs
	for (size_t i = 0; i < tabsCnt; i++)
	{
		os << '\t';
	}
	if (withKey > 0 && key.length() != 0)
		os << '"' << getKey() << '"' << ":";
	printValue(os);
}
void JSONSimpleValue::save(MyString& path, std::ostream& ofs, bool& success) const
{
	if (path == key) {
		success = true;
		int withKey = 1;
		print(ofs, withKey);
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
void JSONSimpleValue::searchKey(const MyString& _key, bool& success) const
{
	if ((_key[_key.length() - 1] == '*' && stringBeginsWith(this->getKey(), _key)) || this->key == _key)
	{
		std::cout << '"' << value << '"';
		std::cout << ',' << std::endl;
		success = true;
	}
}
bool JSONSimpleValue::deleteValue(MyString& path, bool& success)
{
	success = (path == key);
	return success;
}
void JSONSimpleValue::create(MyString& path, const char* value, bool& success)
{}
void JSONSimpleValue::create(MyString& path, const JSON* element, bool& success)
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


