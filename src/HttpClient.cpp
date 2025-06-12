#include "HttpClient.hpp"
#include <curl/curl.h>
#include <stdexcept>
#include <utility>

static size_t writeBodyCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
	// TODO
	throw new std::runtime_error("not implement");
}

static size_t writeHeaderCallback(char* buffer, size_t size, size_t nitems, void* userdata) {
	// TODO
	throw new std::runtime_error("not implement");
}

HttpResponse HttpClient::perform(const Request& req) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		throw std::runtime_error("initialize CURL");
	}

	HttpResponse response{};

	curl_easy_setopt(curl, CURLOPT_URL, req.getUrl().c_str());

	if (req.getMethod() == HttpMethod::POST) {
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.getBody().c_str());
	}

	struct curl_slist* slist = nullptr;
	for (const std::pair<std::string, std::string>& header : req.getHeaders()) {
		const std::string reqHeaderStr = header.first + ": " + header.second;
		slist = curl_slist_append(slist, reqHeaderStr.c_str());
	}
	if (slist) {
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
	}

	// TODO コールバック設定
}