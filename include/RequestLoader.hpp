#pragma once

#include "Request.hpp"
#include <string>
#include <vector>

class RequestLoader {
public:
	// 指定したYAMLファイルを読み込む
	static std::vector<Request> loadFromYaml(const std::string& path);
};