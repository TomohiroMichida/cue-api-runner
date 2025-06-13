#include "Request.hpp"

Request::Request(
	const std::string& name,
	const HttpMethod method,
	const std::string& url,
	const std::map<std::string, std::string>& headers,
	const std::string& body)
	: mName(name),
	mMethod(method),
	mUrl(url),
	mHeaders(headers),
	mBody(body)
{}

const std::string& Request::getName() const {
	return mName;
}

const HttpMethod Request::getMethod() const {
	return mMethod;
}

const std::string& Request::getUrl() const {
	return mUrl;
}

const std::map<std::string, std::string>& Request::getHeaders() const {
	return mHeaders;
}

const std::string& Request::getBody() const {
	return mBody;
}