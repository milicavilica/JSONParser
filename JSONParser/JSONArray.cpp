#include "JSONArray.h"


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
	if (stringBeginsWith(this->key, _key)) {
		print();
		std::cout << ',' << std::endl;
		return;
	}
	for (size_t i = 0; i < size; i++)
	{
		data[i]->searchKey(_key);
	}
}
const JSON* JSONArray::findElem(MyString& path) const
{
	int i = 0;
	while (path[i] != '/')
		i++;
	path = path.substr(i + 1, path.length() - i - 1);
	for (size_t i = 0; i < size; i++)
	{
		this->data[i]->findElem(path);
	}
	return nullptr;
}
void JSONArray::setValue(const MyString& newVal)
{
	free();
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

JSONArray::~JSONArray()
{
	free();
}