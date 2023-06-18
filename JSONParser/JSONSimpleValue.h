#pragma once
#include "MyString.h"
#include "JSON.h"

class JSONSimpleValue : public JSON
{
	MyString value;
public:
	JSONSimpleValue() = default;
	JSONSimpleValue(const MyString& key, const MyString& val);
	const MyString& getValue() const;

	void searchKey(const MyString& _key) const override;

	void print(std::ostream& os, unsigned tabsCnt = 0) const override;
	void printValue(std::ostream& os) const override;

	bool set(MyString& path, const char* newValue, bool& success) override;
	bool set(MyString& path, const JSON* element, bool& success) override;

	bool deleteValue(MyString& path) override;
	void create(MyString& path, const char* value) override;
	void create(MyString& path, const JSON* element) override;

	void save(MyString& path, std::ostream& ofs, bool& success) const override;

	JSON* findElement(MyString& path) override;
	JSON* clone() const override;
	char getType() const override;

};