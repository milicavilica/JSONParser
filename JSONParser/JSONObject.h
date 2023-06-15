#pragma once
#include "JSON.h"
#include "MyString.h"

class JSONObject : public JSON
{
	JSON** value;/// 
	size_t capacity = 8;
	size_t size = 0;
	void free();
	void copyFrom(const JSONObject& other);
	void moveFrom(JSONObject&& other);
	void resize(size_t newCap);
public:
	JSONObject();
	JSONObject(const MyString& _key);
	JSONObject(const JSONObject& other);
	JSONObject(JSONObject&& other);

	JSONObject& operator=(const JSONObject& other);
	JSONObject& operator=(JSONObject&& other);

	void print(unsigned tabsCnt = 0) const override;
	void setValue(const MyString& newVal) override;
	void searchKey(const MyString& _key) const override;
	JSON* clone() const override;
	const JSON* findElem(MyString& path) const override;
	char getType() const override;
	void printValue() const override;
	void addElement(const JSON* el);

	~JSONObject();
};