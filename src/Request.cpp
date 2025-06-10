#include "Request.hpp"

Request::Request(
	const std::string& name,
	HttpMethod method,
	const std::string& url,
	const std::map<std::string, std::string>& headers,
	const std::string& body)
	: mName(name),
	mMethod(method),
	mUrl(url),
	mHeaders(headers),
	mBody(body)
{}