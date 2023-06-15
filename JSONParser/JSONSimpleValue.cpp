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
	if (getKey() != "")
		std::cout << '"' <<  getKey()  << '"' << ":";
	std::cout << '"' << getValue() << '"';
}

JSON* JSONSimpleValue::clone() const
{
	return new JSONSimpleValue(*this);
}
void JSONSimpleValue::searchKey(const MyString& _key) const
{
	if (stringBeginsWith(this->getKey(), _key)) {
		std::cout << '"' << value << '"';
		std::cout << ',' << std::endl;
	}
}
void JSONSimpleValue::printValue() const
{
	std::cout << '"' << value << '"';
}
char JSONSimpleValue::getType() const
{
	return SIMPLE_DATA;
}


