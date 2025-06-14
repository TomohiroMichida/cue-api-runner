#pragma once

#include "Request.hpp"
#include <string>
#include <map>

struct HttpResponse {
	long statusCode;
	std::map<std::string, std::string> headers;
	std::string body;
	double totalTime;
};

class HttpClient {
public:
	static HttpResponse perform(const Request& req);
};