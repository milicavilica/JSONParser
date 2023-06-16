#pragma once
#include "JSON.h"
#include "MyString.h"

class JSONArray : public JSON
{
	JSON** data;
	size_t size = 0;
	size_t capacity = 8;

	void free();
	void copyFrom(const JSONArray& other);
	void moveFrom(JSONArray&& other);
	void resize(size_t newCap);
public:
	JSONArray();
	JSONArray(const MyString& key);
	JSONArray(const JSONArray& other);
	JSONArray(JSONArray&& other);

	JSONArray& operator=(const JSONArray& other);
	JSONArray& operator=(JSONArray&& other);

	void print(unsigned tabsCnt = 0) const override;
	JSON* clone() const override;
	char getType() const override;
	void searchKey(const MyString& _key) const override;
	bool deleteValue(MyString& path) override;
	void create(MyString& path, const char* value) override;
	void addElement(const JSON* el);
	void printValue() const override;
	bool set(MyString& path, const char* newValue) override;

	~JSONArray();
};