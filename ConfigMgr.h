#pragma once
#include "Singleton.h"
class Section {
public:
	std::string operator[](std::string);
	void set_value(std::string key,std::string value);
private:
	std::map<std::string, std::string> _section_data;
};
class ConfigMgr
{
public:

	Section operator[](std::string);
	static ConfigMgr& Inst();
private:
	static ConfigMgr* inst;
	ConfigMgr();
	std::map<std::string, Section> _section_map;
};

