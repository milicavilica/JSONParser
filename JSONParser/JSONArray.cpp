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
void JSONArray::print(unsigned tabsCnt) const
{
	for (size_t i = 0; i < tabsCnt; i++)
	{
		std::cout << '\t';
	}
	if (getKey() != "") 
		std::cout << '"' << getKey() << '"' << ':';
	
	std::cout << '[' << std::endl;
	
	for (size_t i = 0; i < size; i++)
	{
		data[i]->print(tabsCnt + 1);
		if(i != size - 1)
			std::cout << ',' << std::endl;
	}
	std::cout << std::endl;
	for (size_t i = 0; i < tabsCnt; i++)
	{
		std::cout << '\t';
	}
	std::cout<< ']';
}
JSON* JSONArray::clone() const
{
	return new JSONArray(*this);
}
void JSONArray::printValue() const
{
	for (size_t i = 0; i < size; i++)
	{
		data[i]->printValue();
		if (i != size - 1)
			std::cout << ',' << std::endl;
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
		print();
		std::cout << ',' << std::endl;
		return;
	}
	for (size_t i = 0; i < size; i++)
	{
		data[i]->searchKey(_key);
	}
}
bool JSONArray::set(MyString& path, const char* newValue)
{
	if (path == key)
		return true;

	if (key.length() != 0)
	{
		int j = 0;
		while (path[j] != '/' && path[j] != '\0')
			j++;
		if (key != path.substr(0, j))
			return false;
		path = path.substr(j + 1, path.length() - j - 1);
	};

	for (size_t i = 0; i < size; i++)
	{
		data[i]->set(path, newValue);
	}

	return false;
	

}
bool JSONArray::deleteValue(MyString& path)
{
	if (path == key)
		return true;

	if (key.length() != 0)
	{
		int j = 0;
		while (path[j] != '/' && path[j] != '\0')
			j++;
		if (key != path.substr(0, j))
			return false;
		path = path.substr(j + 1, path.length() - j - 1);
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

void JSONArray::create(MyString& path, const char* _value)
{
	if (path == key) {
		std::stringstream ss(_value);
		addElement(factory(_value[0], ss));
	}
	else
		throw std::exception("incorrect path!");
}

JSONArray::~JSONArray()
{
	free();
}