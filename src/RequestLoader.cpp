#include "RequestLoader.hpp"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string_view>

static constexpr std::string_view METHOD_GET{ "GET" };
static constexpr std::string_view METHOD_POST{ "POST" };

static HttpMethod parseMethod(const std::string& method) {
	if (method == METHOD_GET) return HttpMethod::GET;
	if (method == METHOD_POST) return HttpMethod::POST;

	std::cerr << "[Warning] Unknown Http method: " << method << ", defaulting to GET" << std::endl;
	return HttpMethod::GET;
}

std::vector<Request> RequestLoader::loadFromYaml(const std::string& path) {
	// TODO
}