#pragma once

#include <string>
#include <map>

enum class HttpMethod { GET, POST };

class Request {
public:
	Request(
		const std::string& name,
		const HttpMethod method,
		const std::string& url,
		const std::map<std::string, std::string>& headers = {},
		const std::string& body = "");

	const std::string& getName() const;
	const HttpMethod getMethod() const;
	const std::string& getUrl() const;
	const std::map<std::string, std::string>& getHeaders() const;
	const std::string& getBody() const;

private:
	std::string mName;
	HttpMethod mMethod;
	std::string mUrl;
	std::map<std::string, std::string> mHeaders;
	std::string mBody;
};