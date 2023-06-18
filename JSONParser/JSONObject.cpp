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

void JSONObject::print(std::ostream& os, int& withKey, unsigned tabsCnt) const
{
	//printing tabs
	for (size_t i = 0; i < tabsCnt; i++)
	{
		os << '\t';
	}
	if (withKey > 0 && getKey() != "")
		os << '"' << getKey() << '"' << ':';
	os << '{' << std::endl;

	withKey++;
	for (size_t i = 0; i < size; i++)
	{
		value[i]->print(os, withKey, tabsCnt + 1);
		if (i != size - 1)
			os << ',' << std::endl;
	}
	//printing tabs
	os << std::endl;
	for (size_t i = 0; i < tabsCnt; i++)
	{
		os << '\t';
	}
	os << '}';
}
JSON* JSONObject::clone() const
{
	return new JSONObject(*this);
}
char JSONObject::getType() const
{
	return OBJECT;
}
void JSONObject::searchKey(const MyString& _key, bool& success) const
{
	if ((_key[_key.length() - 1] == '*' && stringBeginsWith(this->getKey(), _key)) || this->key == _key) {
		int withKey = 0;
		print(std::cout, withKey);
		std::cout << ',' << std::endl;
		success = true;
		return;
	}
	for (size_t i = 0; i < size; i++)
	{
		value[i]->searchKey(_key, success);
	}
}

void JSONObject::save(MyString& path, std::ostream& ofs, bool& success) const
{
	if (path == key) {
		success = true;
		int withKey = 1;
		print(ofs, withKey);
	}

	if (key.length() != 0 && countSlashes(path) > 0)
	{
		cutPathAndCheckKey(path, key);
	}

	for (size_t i = 0; i < size; i++)
	{
		this->value[i]->save(path, ofs, success);
	}
}

bool JSONObject::set(MyString& path, const char* newValue, bool& success)
{
	std::stringstream ss(newValue);
	JSON* element = factory(newValue[0], ss);//->clone();
	MyString elementKey = path;
	while (countSlashes(elementKey) != 0)
	{
		cutPath(elementKey);
	}
	element->setKey(elementKey);
	return set(path, element, success);
}

bool JSONObject::set(MyString& path, const JSON* element, bool& success)
{
	if (path == key)
		return true;

	if (key.length() != 0 && countSlashes(path) > 0)
	{
		cutPathAndCheckKey(path, key);
	};

	
	for (size_t i = 0; i < size; i++)
	{
		if (this->value[i]->set(path, element, success))
		{
			this->value[i]->~JSON();
			this->value[i] = element->clone();
			success = true;
			return false;
		}
	}
	return false;
}
bool JSONObject::deleteValue(MyString& path, bool& success)
{
	if (path == key) {
		success = true;
		return true;
	}

	if (key.length() != 0)
	{
		if (!cutPathAndCheckKey(path, key))
			return false;
	};


	for (size_t i = 0; i < size; i++)
	{
		if (this->value[i]->deleteValue(path, success))
		{
			this->value[i]->~JSON();
			for (size_t j = i; j < size - 1; j++)
			{
				this->value[j] = this->value[j + 1];
			}
			this->value[size--] = nullptr;
			std::cout << "Element successfully deleted!" << std::endl;
			return false;
		}
	}
	return false;
}
void JSONObject::create(MyString& path, const char* newValue, bool& success)
{
	std::stringstream ss(newValue);
	JSON* element = factory(newValue[0], ss);
	MyString elementKey = path;
	while (countSlashes(elementKey) != 0)
	{
		cutPath(elementKey);
	}
	element->setKey(elementKey);
	create(path, element, success);
}
void JSONObject::create(MyString& path, const JSON* element, bool& success)
{
	if (path == key)
		throw std::exception("An object with this key already exists!");

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
				throw std::exception("An object with this key already exists!");
		}
		addElement(element);
		success = true;
	}
	else {
		for (size_t i = 0; i < size; i++)
		{
			this->value[i]->create(path, element, success);
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
void JSONObject::printValue(std::ostream& os) const
{
	for (size_t i = 0; i < size; i++)
	{
		value[i]->printValue(os);
		if (i != size - 1)
			os << ',' << std::endl;
	}
}
JSONObject::~JSONObject()
{
	free();
}