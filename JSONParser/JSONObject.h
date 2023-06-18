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

	void print(std::ostream& os, int& withKey, unsigned tabsCnt = 0) const override;

	void searchKey(const MyString& _key, bool& success) const override;
	JSON* clone() const override;

	bool set(MyString& path, const char* newValue, bool& success) override;
	bool set(MyString& path, const JSON* element, bool& success) override;

	bool deleteValue(MyString& path, bool& success) override;

	void create(MyString& path, const char* value, bool& success) override;
	void create(MyString& path, const JSON* element, bool& success) override;

	void save(MyString& path, std::ostream& ofs, bool& success) const override;

	JSON* findElement(MyString& path) override;

	char getType() const override;
	void printValue(std::ostream& os) const override;

	void addElement(const JSON* el);

	~JSONObject();
};