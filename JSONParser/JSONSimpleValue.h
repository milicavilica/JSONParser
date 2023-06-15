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
	void print(unsigned tabsCnt = 0) const override;
	void printValue() const override;
	JSON* clone() const override;
	char getType() const override;
};