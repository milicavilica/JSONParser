#include "JSON.h"

JSON::JSON(const MyString& _key) : key(_key)
{}


const MyString& JSON::getKey() const
{
	return key;
}
void JSON::setKey(const MyString& str)
{
	key = std::move(str);
}