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
	if (key.length() != 0)
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
bool JSONObject::set(MyString& path, const char* newValue, bool& succes)
{
	std::stringstream ss(newValue);
	JSON* element = factory(newValue[0], ss);//->clone();
	MyString elementKey = path;
	while (countSlashes(elementKey) != 0)
	{
		cutPath(elementKey);
	}
	element->setKey(elementKey);
	return set(path, element, succes);
}

bool JSONObject::set(MyString& path, const JSON* element, bool& succes)
{
	if (path == key)
		return true;

	if (key.length() != 0 && countSlashes(path) > 0)
	{
		cutPathAndCheckKey(path, key);
	};

	
	for (size_t i = 0; i < size; i++)
	{
		if (this->value[i]->set(path, element, succes))
		{
			this->value[i]->~JSON();
			this->value[i] = element->clone();
			succes = true;
			return false;
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
		if (!cutPathAndCheckKey(path, key))
			return false;
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
			return false;
		}
	}
	return false;
}
void JSONObject::create(MyString& path, const char* newValue)
{
	std::stringstream ss(newValue);
	JSON* element = factory(newValue[0], ss);
	element->setKey(path);
	create(path, element);
}
void JSONObject::create(MyString& path, const JSON* element)
{
	if (path == key)
		throw std::exception("an object with this key already exists!");

	if (key.length() != 0)
	{
		cutPathAndCheckKey(path, key);
	};


	int slashCount = countSlashes(path);
	/*int i = 0;
	while (path[i] != '\0')
	{
		if (path[i++] == '/')
			slashCount++;
	}*/
	if (slashCount == 0)
	{
		for (size_t i = 0; i < size; i++)
		{
			if(this->value[i]->getKey() == path)
				throw std::exception("an object with this key already exists!");
		}
		addElement(element);
	}
	else {
		for (size_t i = 0; i < size; i++)
		{
			this->value[i]->create(path, element);
		}
	}
}
JSON* JSONObject::findElement(MyString& path)
{
	if (path == key)
		return this;
	if (key.length() != 0)
	{
		cutPathAndCheckKey(path, key);
	};
	JSON* element = nullptr;
	for (size_t i = 0; i < size; i++)
	{
		element = this->value[i]->findElement(path);
		if (element != nullptr) {
			return element->clone();
		}
	}
	return element;
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