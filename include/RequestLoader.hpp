#pragma once

#include "Request.hpp"
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>

enum class HttpMethod;

class RequestLoader {
public:
	// 指定したYAMLファイルを読み込む
	static std::vector<Request> loadFromYaml(const std::string& path);

private:
	static HttpMethod parseMethod(const std::string& method);
	static std::string expectString(const YAML::Node& parent, const char* key);
};