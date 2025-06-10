#pragma once

#include "Request.hpp"
#include <string>
#include <vector>

class RequestLoader {
public:
	// �w�肵��YAML�t�@�C����ǂݍ���
	static std::vector<Request> loadFromYaml(const std::string& path);
};