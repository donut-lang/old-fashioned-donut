/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <cinamo/Url.h>
#include <cinamo/String.h>
#include <cinamo/FormPayload.h>
#include <cinamo/Exception.h>

namespace cinamo {

FormPayload::FormPayload() {
}

void FormPayload::readURLEncoded(const std::string& data)
{
	/* ☆車☆輪☆の☆再☆発☆明☆ */
	std::vector<std::string> elements(cinamo::split(data, "&"));
	for(std::vector<std::string>::const_iterator it = elements.begin(); it != elements.end(); ++it){
		std::string const& str = *it;
		const size_t split = str.find("=");
		std::string key=str.substr(0, split);
		std::string val=str.substr(split+1);
		key = cinamo::url::decodeForm(key);
		val = cinamo::url::decodeForm(val);
		this->map.insert(std::pair<std::string, std::string>(key, val));
	}
}

size_t FormPayload::size() const
{
	return map.size();
}

bool FormPayload::has(const std::string& key) const
{
	return map.find(key) != map.end();
}
std::string FormPayload::getString(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator val = map.find(key);
	if(val == map.end()){
		CINAMO_EXCEPTION(Exception, "There is no item for key: %s", key.c_str());
		return std::string();
	}else{
		return val->second;
	}
}

#if defined(HAVE_STRTOLL)
long long FormPayload::getLong(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator val = map.find(key);
	if(val == map.end()){
		CINAMO_EXCEPTION(Exception, "There is no item for key: %s", key.c_str());
		return 0;
	}else{
		return strtoll(val->second.c_str(), 0, 10);
	}
}
long long FormPayload::optLong(const std::string& key, long long def) const
{
	if(this->has(key)){
		return getLong(key);
	}else{
		return def;
	}
}
#else
long FormPayload::getLong(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator val = map.find(key);
	if(val == map.end()){
		CINAMO_EXCEPTION(Exception, "There is no item for key: %s", key.c_str());
		return 0;
	}else{
		return strtol(val->second.c_str(), 0, 10);
	}
}
long FormPayload::optLong(const std::string& key, long def) const
{
	if(this->has(key)){
		return getLong(key);
	}else{
		return def;
	}
}
#endif

bool FormPayload::getBool(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator val = map.find(key);
	if(val == map.end()){
		CINAMO_EXCEPTION(Exception, "There is no item for key: %s", key.c_str());
		return 0;
	}else{
		std::string v(val->second);
		char* end;
		unsigned long num = strtol(v.c_str(), &end, 0);
		if(*end == '\0'){
			return num != 0;
		}else{
			std::transform(v.begin(), v.end(), v.begin(), ::tolower);
			return v == "true";
		}
	}
}

std::string FormPayload::optString(const std::string& key, const std::string& def) const
{
	if(this->has(key)){
		return getString(key);
	}else{
		return def;
	}
}
bool FormPayload::optBool(const std::string& key, bool def) const
{
	if(this->has(key)){
		return getBool(key);
	}else{
		return def;
	}
}

FormPayload::Iterator FormPayload::begin() const
{
	return map.begin();
}

FormPayload::Iterator FormPayload::end() const
{
	return map.end();
}

}
