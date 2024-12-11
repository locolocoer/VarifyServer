#include "ConfigMgr.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>
std::string Section::operator[](std::string key)
{
	return _section_data[key];
}

void Section::set_value(std::string key,std::string value)
{
	_section_data[key] = value;
}

Section ConfigMgr::operator[](std::string key)
{
	return _section_map[key];
}

ConfigMgr& ConfigMgr::Inst()
{
	if (inst == nullptr) {
		inst = new ConfigMgr();
	}
	return *inst;
}

ConfigMgr::ConfigMgr()
{
	auto currentPath = boost::filesystem::current_path();
	auto iniPath = currentPath / "config.ini";
	std::cout << "Config file path: " << iniPath.string() << std::endl;
	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(iniPath.string(), pt);
	for (auto section_pair : pt) {
		Section sec;
		auto key = section_pair.first;
		for (auto section : section_pair.second) {
			auto secKey = section.first;
			auto secValue = section.second.get_value<std::string>();
			sec.set_value(secKey, secValue);
		}
		_section_map[key] = sec;
	}
}

ConfigMgr* ConfigMgr::inst = nullptr;