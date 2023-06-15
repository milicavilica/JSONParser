#include "JSONObject.h"

void JSONObject::free()
{
	if (value != nullptr)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete value[i];
		}
		delete[] value;
		size = capacity = 0;
	}
}
void JSONObject::copyFrom(const JSONObject& other)
{
	key = other.key;
	value = new JSON * [other.capacity];
	size = other.size;
	capacity = other.capacity;
	for (size_t i = 0; i < size; i++)
	{
		value[i] = other.value[i]->clone();
	}

}

void JSONObject::moveFrom(JSONObject&& other)
{

	size = other.size;
	capacity = other.capacity;
	value = other.value;
	other.value = nullptr;
	key = std::move(other.key);

}

void JSONObject::resize(size_t newCap)
{
	if (newCap < size)
		throw std::invalid_argument("new capacity is less than current size");

	JSON** newData = new JSON * [newCap] {nullptr};
	for (size_t i = 0; i < size; i++)
	{
		newData[i] = value[i];
	}
	delete[] value;
	value = newData;
	capacity = newCap;
}
JSONObject::JSONObject()
{
	value = new JSON * [capacity];
}
JSONObject::JSONObject(const MyString& _key)
{
	value = new JSON * [capacity];
}
JSONObject::JSONObject(const JSONObject& other)
{
	copyFrom(other);
}
JSONObject::JSONObject(JSONObject&& other)
{
	moveFrom(std::move(other));
}

JSONObject& JSONObject::operator=(const JSONObject& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
JSONObject& JSONObject::operator=(JSONObject&& other)
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}
void JSONObject::addElement(const JSON* el)
{
	if (size == capacity)
		resize(capacity * 2);

	if(el != nullptr)
		value[size++] = el->clone();
}
void JSONObject::print(unsigned tabsCnt) const
{
	//printing tabs
	for (size_t i = 0; i < tabsCnt; i++)
	{
		std::cout << '\t';
	}
	if (getKey() != "")
		std::cout << '"' << getKey() << '"' << ':';
	std::cout << '{' << std::endl;
	for (size_t i = 0; i < size; i++)
	{
		value[i]->print(tabsCnt + 1);
		if (i != size - 1)
			std::cout << ',' << std::endl;
	}
	//printing tabs
	std::cout << std::endl;
	for (size_t i = 0; i < tabsCnt; i++)
	{
		std::cout << '\t';
	}
	std::cout << '}';
}
JSON* JSONObject::clone() const
{
	return new JSONObject(*this);
}
char JSONObject::getType() const
{
	return OBJECT;
}
void JSONObject::searchKey(const MyString& _key) const
{
	if (stringBeginsWith(this->key, _key)) {
		print();
		std::cout << ',' << std::endl;
		return;
	}
	for (size_t i = 0; i < size; i++)
	{
		value[i]->searchKey(_key);
	}
}
void JSONObject::setValue(const MyString& newVal)
{
	
}
const JSON* JSONObject::findElem(MyString& path) const
{
	int i = 0;
	while (path[i] != '/' && path[i] != '\0')
		i++;
	if (i != path.length())
	{
		MyString currentKey = path.substr(0, i);
		path = path.substr(i + 1, path.length() - i - 1);
		for (size_t i = 0; i < size; i++)
		{
			if (currentKey == this->value[i]->getKey())
				return this->value[i]->findElem(path);
		}
	}
	else
	{
		for (size_t i = 0; i < size; i++)
		{
			if (path == this->value[i]->getKey())
				return this->value[i]->findElem(path);
		}
	}
	return nullptr;
}
void JSONObject::printValue() const
{
	for (size_t i = 0; i < size; i++)
	{
		value[i]->printValue();
		if (i != size - 1)
			std::cout << ',' << std::endl;
	}
}
JSONObject::~JSONObject()
{
	free();
}