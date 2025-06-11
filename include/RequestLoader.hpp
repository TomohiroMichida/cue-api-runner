#pragma once

#include "Request.hpp"
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>

enum class HttpMethod;

class RequestLoader {
public:
	// �w�肵��YAML�t�@�C����ǂݍ���
	static std::vector<Request> loadFromYaml(const std::string& path);

private:
	static HttpMethod parseMethod(const std::string& method);
	static std::string expectString(const YAML::Node& parent, const char* key);
};