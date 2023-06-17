#include "JSONFactory.h"
#include "Helper.h"
#include <fstream>

JSON* factory(const char type, std::istream& ifs)
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
		char buff[1024]{};
		int i = 0;
		int quotesCount = 0;
		size_t currentPos = ifs.tellg();
		bool isJustAValue = false;
		while (true)
		{
			buff[i] = ifs.get();
			if (buff[i] == '\"')
			{
				quotesCount++;
				if (quotesCount == 2)
				{
					if (!isThereAColon(ifs)) {
						isJustAValue = true;
						break;
					}
					else
					{
						char c = ifs.get();
						int charsSkipped = 0;
						while (c == ' ' || c == '\t' || c == '\n')
						{
							c = ifs.get();
							charsSkipped++;
						}
						if (c == '\"')
						{
							ifs.seekg(-2 - charsSkipped, std::ios::cur);
							i++;
							continue;
						}
						else if(c == '[')
						{
							ifs.seekg(currentPos);
							return factory(ARRAY, ifs);
						}
						else
						{
							ifs.seekg(currentPos);
							return factory(OBJECT, ifs);
						}
					}
				}
				if (quotesCount == 4)
					break;
			}
			i++;
		}
		if (isJustAValue)
		{
			removeQuotes(buff);
			return new JSONSimpleValue("", buff);
		}
		char key[1024]{};
		i = 0;
		while (buff[i] != ':')
		{
			key[i] = buff[i];
			i++;
		}
		i++;
		char value[1024]{};
		int j = 0;
		while (buff[i] != '\0')
		{
			value[j++] = buff[i];
			i++;
		}

		removeQuotes(key);
		removeQuotes(value);
		return new JSONSimpleValue(key, value);
		//size_t currentpos = ifs.tellg();
		//char buff[1024] = { '\0' };
		//int i = 0;
		//int quoteCnt = 0;
		//while (true)
		//{
		//	buff[i] = ifs.get();
		//	if (buff[i++] == '\"') {
		//		quoteCnt++;
		//		if()
		//		if (quoteCnt == 2 && !isThereAColon(ifs))
		//		{
		//			break;
		//		}
		//		if (quoteCnt == 4)
		//			break;
		//	}
		//	
		//	/*buff[i] = ifs.get();
		//	if (buff[i] == ']') {
		//		buff[i] = '\0';
		//		ifs.seekg(-1, std::ios::cur);
		//		break;
		//	}
		//	if (buff[i++] == '\"')
		//		quoteCnt++;
		//	if (quoteCnt == 4)
		//		break;*/
		//}
		//i = 0;
		//while (buff[i] != '\0') {
		//	if (buff[i] == ',') {
		//		buff[i] = '\0';
		//		break;
		//	}
		//	if (buff[i] == ARRAY) {
		//		ifs.seekg(currentpos);
		//		return factory(ARRAY, ifs);
		//	}
		//	if (buff[i] == OBJECT) {
		//		ifs.seekg(currentpos);
		//		return factory(OBJECT, ifs);
		//	}
		//	i++;
		//}
		//if (buff[i] != '\0')
		//	break;

		//ifs.seekg(currentpos);

		//char keyOrValueBuff[1024];
		//keyOrValueBuff[0] = '\0';
		//i = 0;
		//while (buff[i] != '\n' && buff[i] != '\0') 
		//{
		//	if (buff[i] == ':') 
		//	{
		//		ifs.seekg(currentpos);
		//		ifs.getline(keyOrValueBuff, 1024, ':');
		//		char c = ifs.peek();
		//		if (c == ARRAY) {
		//			factory(ARRAY, ifs);
		//			break;
		//		}
		//		if (c == OBJECT) {
		//			factory(OBJECT, ifs);
		//			break;
		//		}

		//		char value[1024];
		//		int j = 0;
		//		quoteCnt = 0;
		//		while (true)
		//		{
		//			value[j] = ifs.get();
		//			if (value[j++] == '\"') {
		//				quoteCnt++;
		//				if (quoteCnt == 2 && !isThereAColon(ifs))
		//					break;
		//				if (quoteCnt == 4)
		//					break;
		//			}
		//			
		//			/*if (value[j] == ']') 
		//			{
		//				value[j] = '\0';
		//				ifs.seekg(-1, std::ios::cur);
		//				break;
		//			}
		//			if (value[j++] == '\"')
		//				quoteCnt++;
		//			if (quoteCnt == 2)
		//				break;*/
		//		}

		//		value[j] = '\0';
				/*removeQuotes(keyOrValueBuff);
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
			while (ch != '"')
			{
				keyOrValueBuff[i++] = ch;
				ch = ifs.get();
			}
			keyOrValueBuff[i++] = ch;
			keyOrValueBuff[i] = '\0';
			removeQuotes(keyOrValueBuff);
			return new JSONSimpleValue("", keyOrValueBuff);
			break;*/
		
	}
	default:
		throw std::exception("wrong file format!");
		break;
	}
	
}
