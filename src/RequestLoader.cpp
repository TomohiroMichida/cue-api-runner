#include "RequestLoader.hpp"
#include <yaml-cpp/yaml.h>
#include <map>
#include <iostream>
#include <utility>

static constexpr const char* METHOD_GET = "GET";
static constexpr const char* METHOD_POST = "POST";

static constexpr const char* YAML_CONFIG_ROOT_NAME = "requests";
static constexpr const char* REQUESTS_REQUIRED_FIELD_NAME = "name";
static constexpr const char* REQUESTS_REQUIRED_FIELD_METHOD = "method";
static constexpr const char* REQUESTS_REQUIRED_FIELD_URL = "url";

static constexpr const char* REQUESTS_OPTIONAL_FIELD_HEADERS = "headers";
static constexpr const char* REQUESTS_OPTIONAL_FIELD_BODY = "body";

std::vector<Request> RequestLoader::loadFromYaml(const std::string& path) {
	std::vector<Request> reqList{};

	YAML::Node root;
	try {
		root = YAML::LoadFile(path);
	} catch (const YAML::BadFile& e) {
		std::cerr << "[Error] Yamlファイルの読み込みに失敗しました： " << path << std::endl;
		return reqList;
	}

	const YAML::Node& requestsNode = root[YAML_CONFIG_ROOT_NAME];
	if (!requestsNode || !requestsNode.IsSequence()) {
		std::cerr << "[Error] requestsノードが存在しません： " << path << std::endl;
		return reqList;
	}

	for (const YAML::Node& node : requestsNode) {
		// 必須フィールド
		const std::string name = expectString(node, REQUESTS_REQUIRED_FIELD_NAME);
		const std::string method = expectString(node, REQUESTS_REQUIRED_FIELD_METHOD);
		const std::string url = expectString(node, REQUESTS_REQUIRED_FIELD_URL);

		const HttpMethod parsedMethod = parseMethod(method);
		
		// ヘッダー部
		std::map<std::string, std::string> headers{};
		if (node[REQUESTS_OPTIONAL_FIELD_HEADERS]) {
			for (const std::pair<YAML::Node, YAML::Node>& header : node[REQUESTS_OPTIONAL_FIELD_HEADERS]) {
				if (!header.first.IsScalar() || !header.second.IsScalar()) {
					std::cerr << "[Warning] headerが不正です：" << name << std::endl;
					continue;
				}
				headers.emplace(header.first.Scalar(), header.second.Scalar());
			}
		}

		// body部
		std::string body{};
		if (node[REQUESTS_OPTIONAL_FIELD_BODY]) {
			if (!node[REQUESTS_OPTIONAL_FIELD_BODY].IsScalar()) {
				std::cerr << "[Warning] bodyが文字列ではありません： " << name << std::endl;
			} else {
				body = node[REQUESTS_OPTIONAL_FIELD_BODY].Scalar();
			}
		}

		reqList.emplace_back(name, parsedMethod, url, headers, body);
		std::cout << "YAMLファイル読み込み完了：" << name << std::endl;
	}

	return reqList;
}

HttpMethod RequestLoader::parseMethod(const std::string& method) {
	if (method == METHOD_GET) return HttpMethod::GET;
	if (method == METHOD_POST) return HttpMethod::POST;

	std::cerr << "[Warning] 不正なmethodです： " << method << ", デフォルトで GETを設定します" << std::endl;
	return HttpMethod::GET;
}

std::string RequestLoader::expectString(const YAML::Node& parent, const char* key) {
	const YAML::Node& node = parent[key];
	if (!node || !node.IsScalar()) {
		throw std::runtime_error(std::string("Missing or invalid '") + key + "' field");
	}
	return node.Scalar();
}