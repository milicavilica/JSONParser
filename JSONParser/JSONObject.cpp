#include "JSONObject.h"
#include "JSONFactory.h"
#include <sstream>

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
	if ((_key[_key.length() - 1] == '*' && stringBeginsWith(this->getKey(), _key)) || this->key == _key) {
		print();
		std::cout << ',' << std::endl;
		return;
	}
	for (size_t i = 0; i < size; i++)
	{
		value[i]->searchKey(_key);
	}
}
bool JSONObject::set(MyString& path, const char* newValue)
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
		if (this->value[i]->set(path, newValue))
		{
			this->value[i]->~JSON();
			std::stringstream ss(newValue);
			this->value[i] = factory(newValue[0], ss);
			this->value[i]->setKey(path);
		}
	}
	return false;
}
bool JSONObject::deleteValue(MyString& path)
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
		if (this->value[i]->deleteValue(path))
		{
			this->value[i]->~JSON();
			for (size_t j = i; j < size - 1; j++)
			{
				this->value[j] = this->value[j + 1];
			}
			this->value[size--] = nullptr;
		}
	}
	return false;
}
void JSONObject::create(MyString& path, const char* newValue)
{
	if (path == key)
		throw std::exception("an object with this key already exists!");

	if (key.length() != 0)
	{
		int j = 0;
		while (path[j] != '/' && path[j] != '\0')
			j++;
		if (key != path.substr(0, j))
			throw std::exception("incorrect path!");
		path = path.substr(j + 1, path.length() - j - 1);
	};

	int slashCount = 0;
	int i = 0;
	while (path[i] != '\0')
	{
		if (path[i++] == '/')
			slashCount++;
	}
	if (slashCount == 0)
	{
		std::stringstream ss(newValue);
		addElement(factory(newValue[0], ss));
		this->value[size - 1]->setKey(path);
	}
	else {
		for (size_t i = 0; i < size; i++)
		{
			this->value[i]->create(path, newValue);
		}
	}
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