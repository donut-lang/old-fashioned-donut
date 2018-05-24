/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/String.h>

namespace cinamo {

std::vector<std::string> split(std::string const& str, std::string const& sep)
{
	std::vector<std::string> list;
	if(sep.size() <= 0){
		return list;
	}
	size_t i = 0;
	size_t found = 0;
	while((found = str.find(sep, i)) != std::string::npos){
		if(found != i){
			list.push_back(str.substr(i, found-i));
		}
		i = found+sep.length();
	}
	if(i != str.size()){
		list.push_back(str.substr(i));
	}
	return list;
}

static size_t findFirstOf(std::string const& str, const std::string* sep, size_t n, size_t from, size_t* offset, size_t* seplen)
{
	*offset = std::string::npos;
	for(size_t i = 0;i<n;++i){
		size_t found = str.find(sep[i], from);
		if(sep[i].size() > 0 && found != std::string::npos && (found < *offset || *offset == std::string::npos)){
			*offset = found;
			*seplen = sep[i].size();
		}
	}
	return *offset;
}

namespace internal {
std::vector<std::string> split(std::string const& str, const std::string* sep, size_t n)
{
	std::vector<std::string> list;
	size_t i = 0;
	size_t found = std::string::npos;
	size_t seplen = std::string::npos;
	while((findFirstOf(str, sep, n, i, &found, &seplen)) != std::string::npos){
		if(found != i){
			list.push_back(str.substr(i, found-i));
		}
		i = found+seplen;
	}
	if(i != str.size()){
		list.push_back(str.substr(i));
	}
	return list;
}

}

std::vector<std::string> splitLine(std::string const& str)
{
	static const std::string lineSep[] = {
			"\n",
			"\r"
	};
	return split(str, lineSep);
}
std::vector<std::string> splitSpace(std::string const& str)
{
	static const std::string spaces[] = {
			" ",
			"　"
	};
	return split(str, spaces);
}

}
