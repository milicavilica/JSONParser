#include "JSONArray.h"
#include "JSONFactory.h"
#include <sstream>


JSONArray::JSONArray()
{
	data = new JSON * [capacity];
}

JSONArray::JSONArray(const MyString& key) : JSON(key)
{
	data = new JSON * [capacity];
}
void JSONArray::print(std::ostream& os, unsigned tabsCnt) const
{
	//printing tabs
	for (size_t i = 0; i < tabsCnt; i++)
	{
		os << '\t';
	}
	if (getKey() != "") 
		os << '"' << getKey() << '"' << ':';
	
	os << '[' << std::endl;
	
	for (size_t i = 0; i < size; i++)
	{
		data[i]->print(os, tabsCnt + 1);
		if(i != size - 1)
			os << ',' << std::endl;
	}
	//printing tabs
	os << std::endl;
	for (size_t i = 0; i < tabsCnt; i++)
	{
		os << '\t';
	}
	os << ']';
}
JSON* JSONArray::clone() const
{
	return new JSONArray(*this);
}
void JSONArray::printValue(std::ostream& os) const
{
	for (size_t i = 0; i < size; i++)
	{
		data[i]->printValue(os);
		if (i != size - 1)
			os << ',' << std::endl;
	}
}

void JSONArray::addElement(const JSON * el)
{
	if (size == capacity)
		resize(capacity * 2);
	if(el != nullptr)
		data[size++] = el->clone();
}

void JSONArray::free()
{
	if (data != nullptr)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete data[i];
		}
		delete[] data;
		size = capacity = 0;
	}
}
void JSONArray::copyFrom(const JSONArray& other)
{
	key = other.key;
	data = new JSON* [other.capacity];
	size = other.size;
	capacity = other.capacity;
	for (size_t i = 0; i < size; i++)
	{
		data[i] = other.data[i]->clone();
	}

}
void JSONArray::moveFrom(JSONArray&& other)
{
	key = std::move(other.key);
	size = other.size;
	capacity = other.capacity;
	data = other.data;
	other.data = nullptr;

}
void JSONArray::resize(size_t newCap)
{
	if (newCap < size)
		throw std::invalid_argument("new capacity is less than current size");

	JSON** newData = new JSON * [newCap] {nullptr};
	for (size_t i = 0; i < size; i++)
	{
		newData[i] = data[i];
	}
	delete[] data;
	data = newData;
	capacity = newCap;
}

JSONArray::JSONArray(const JSONArray& other)
{
	copyFrom(other);
}
JSONArray::JSONArray(JSONArray&& other)
{
	moveFrom(std::move(other));
}
char JSONArray::getType() const
{
	return ARRAY;
}
void JSONArray::searchKey(const MyString& _key) const
{	
	if ((_key[_key.length() - 1] == '*' && stringBeginsWith(this->getKey(), _key)) || this->key == _key) {
		print(std::cout);
		std::cout << ',' << std::endl;
		return;
	}
	for (size_t i = 0; i < size; i++)
	{
		data[i]->searchKey(_key);
	}
}
void JSONArray::save(MyString& path, std::ostream& ofs, bool& success) const
{
	if (path == key) {
		success = true;
		print(ofs);
	}
	if (countSlashes(path) == 0)
		return;
	if (key.length() != 0 )
	{
		if (!cutPathAndCheckKey(path, key))
			return;
	}

	for (size_t i = 0; i < size; i++)
	{
		data[i]->save(path, ofs, success);
	}
}

bool JSONArray::set(MyString& path, const char* newValue, bool& success)
{
	std::stringstream ss(newValue);
	JSON* element = factory(newValue[0], ss);
	element->setKey(path);
	return set(path, element, success);

}
bool JSONArray::set(MyString& path, const JSON* element, bool& success)
{
	if (path == key)
		return true;
	
	if (key.length() != 0 && countSlashes(path) > 0)
	{
		if (!cutPathAndCheckKey(path, key))
			return false;
	}

	for (size_t i = 0; i < size; i++)
	{
		data[i]->set(path, element, success);
	}

	return false;
}
bool JSONArray::deleteValue(MyString& path)
{
	if (path == key)
		return true;

	if (key.length() != 0 && countSlashes(path) > 0)
	{
		if (!cutPathAndCheckKey(path, key))
			return false;
	};

	for (size_t i = 0; i < size; i++)
	{
		data[i]->deleteValue(path);
	}

	return false;
}

JSONArray& JSONArray::operator=(const JSONArray& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
JSONArray& JSONArray::operator=(JSONArray&& other)
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

JSON* JSONArray::findElement(MyString& path)
{
	if (key == path) {
		return this->clone();
	}
	return nullptr;
}

void JSONArray::create(MyString& path, const char* _value)
{
	if (path == key) {
		std::stringstream ss(_value);
		addElement(factory(_value[0], ss));
	}
	else
		throw std::exception("incorrect path!");
}
void JSONArray::create(MyString& path, const JSON* element)
{
	if (path == key) {
		
		addElement(element);
	}
	
}

JSONArray::~JSONArray()
{
	free();
}