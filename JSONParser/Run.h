#pragma once
#include "JSONParser.h"

class Run {
	static void runValidate(const JSONParser& obj, int& reprintMenu);
	static void runOpenFile(JSONParser& obj, int& reprintMenu);
	static void runSearchKey(const JSONParser& obj, int& reprintMenu);
	static void runSet(JSONParser& obj, int& reprintMenu);
	static void runCreate(JSONParser& obj, int& reprintMenu);
	static void runDelete(JSONParser& obj, int& reprintMenu);
	static void runMove(JSONParser& obj, int& reprintMenu);
	static void runSave(const JSONParser& obj, int& reprintMenu);
	static void runSaveAs(const JSONParser& obj, int& reprintMenu);
public:
	static void run();
};