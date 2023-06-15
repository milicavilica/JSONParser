#include "JSONFactory.h"
#include "Helper.h"
#include <fstream>

JSON* factory(const char type, std::ifstream& ifs)
{
	switch (type)
	{
	case OBJECT:
	{
		JSONObject obj;
		char c = ifs.peek();
		if (c == SIMPLE_DATA) {
			char key[1024];
			ifs.getline(key, 1024, ':');
			removeQuotes(key);
			obj.setKey(key);
		}
		c = ifs.get();
		while (c == ' ' || c == '\t')
			c = ifs.get();
		while (true)
		{
			c = ifs.get();

			while (c == ' ' || c == '\n' || c == '\t' || c == ',') {
				c = ifs.get();
			}
			if (c == '}')
				break;
			ifs.seekg(-1, std::ios::cur);
			obj.addElement(factory(c, ifs));
		}
		return new JSONObject(std::move(obj));
		break;
	}
	case ARRAY:
	{
		JSONArray arr;
		char c = ifs.peek();
		if (c == SIMPLE_DATA) {
			char key[1024];
			ifs.getline(key, 1024, ':');
			removeQuotes(key);
			arr.setKey(key);
		}
		c = ifs.get();
		while (c == ' ' || c == '\t')
			c = ifs.get();
		while (true)
		{
			c = ifs.get();

			while (c == ' ' || c == '\n' || c == '\t' || c == ',') {
				c = ifs.get();
			}
			if (c == ']')
				break;
			ifs.seekg(-1, std::ios::cur);
			arr.addElement(factory(c, ifs));
		}

		return new JSONArray(std::move(arr));
		break;
	}
	case SIMPLE_DATA:
	{
		size_t currentpos = ifs.tellg();
		char buff[1024] = {'\0'};
		ifs.getline(buff, 1024);
		int i = 0;
		while (buff[i] != '\0') {
			if (buff[i] == ',') {
				buff[i] = '\0';
				break;
			}
			if (buff[i] == ARRAY) {
				ifs.seekg(currentpos);
				return factory(ARRAY, ifs);
			}
			if (buff[i] == OBJECT) {
				ifs.seekg(currentpos);
				return factory(OBJECT, ifs);
			}
			i++;
		}
		if (buff[i] != '\0')
			break;

		ifs.seekg(currentpos);
		i = 0;

		char keyOrValueBuff[1024];
		keyOrValueBuff[0] = '\0';
		ifs.getline(buff, 1024);
		while (buff[i] != '\n' && buff[i] != '\0' ) {
			if (buff[i] == ':') {
				ifs.seekg(currentpos);
				ifs.getline(keyOrValueBuff, 1024, ':');
				char c = ifs.peek();
				if (c == ARRAY) {
					factory(ARRAY, ifs);
					break;
				}
				if (c == OBJECT) {
					factory(OBJECT, ifs);
					break;
				}
				char value[1024];
				ifs.getline(value, 1024, '\n');
				for (size_t i = strlen(value); i > 0; i--)
				{
					if (value[i] != '"')
						value[i] = '\0';
					else
						break;
				}
				removeQuotes(keyOrValueBuff);
				removeQuotes(value);
				return new JSONSimpleValue(keyOrValueBuff, value);
			}
			i++;
		}
		i = 1;
		ifs.seekg(currentpos);
		char ch = ifs.get();
		keyOrValueBuff[0] = ch;
		ch = ifs.get();
		while (ch != '"') {
			keyOrValueBuff[i++] = ch;
			ch = ifs.get();
		}
		keyOrValueBuff[i++] = ch;
		keyOrValueBuff[i] = '\0';
		removeQuotes(keyOrValueBuff);
		return new JSONSimpleValue("", keyOrValueBuff);
		break;
	}
	default:
		throw std::exception("wrong file format!");
		break;
	}
}
