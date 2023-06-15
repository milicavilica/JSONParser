#pragma once
#include "JSON.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONSimpleValue.h"

JSON* factory(const char type, std::ifstream& ifs);