#include "HttpClient.hpp"
#include <curl/curl.h>
#include <stdexcept>
#include <utility>

static size_t writeBodyCallback(char* bodyPtr, size_t size, size_t nitems, void* userdata) {
	std::string* const body = static_cast<std::string*>(userdata);
	body->append(bodyPtr, size * nitems);

	return size * nitems;
}

static size_t writeHeaderCallback(char* buffer, size_t size, size_t nitems, void* userdata) {
	std::map<std::string, std::string>* const headers = static_cast<std::map<std::string, std::string>*>(userdata);
	const std::string line(buffer, size * nitems);
	const size_t pos = line.find(':');
	if (pos != std::string::npos) {
		// �w�b�_�[���ƒl�𕪊�
		const std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);
		if (!value.empty() && value.front() == ' ') value.erase(0, 1);
		if (!value.empty() && (value.back() == '\r' || value.back() == '\n')) value.pop_back();
		(* headers)[key] = value;
	}

	return size * nitems;
}

HttpResponse HttpClient::perform(const Request& req) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		throw std::runtime_error("initialize CURL");
	}

	HttpResponse response{};

	// URL
	curl_easy_setopt(curl, CURLOPT_URL, req.getUrl().c_str());

	// ���\�b�h
	if (req.getMethod() == HttpMethod::POST) {
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.getBody().c_str());
	}

	// �w�b�_�[
	struct curl_slist* slist = nullptr;
	for (const std::pair<std::string, std::string>& header : req.getHeaders()) {
		const std::string reqHeaderStr = header.first + ": " + header.second;
		slist = curl_slist_append(slist, reqHeaderStr.c_str());
	}
	if (slist) {
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
	}

	// �R�[���o�b�N�ݒ�
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeBodyCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writeHeaderCallback);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.headers);

	// ���N�G�X�g���s
	const CURLcode code = curl_easy_perform(curl);
	if (code != CURLE_OK) {
		curl_slist_free_all(slist);
		curl_easy_cleanup(curl);
		throw std::runtime_error(std::string("CURL error: ") + curl_easy_strerror(code));
	}

	// �X�e�[�^�X�R�[�h
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.statusCode);

	// �J��(unique_ptr�Ń��b�v���������悳����)
	curl_slist_free_all(slist);
	curl_easy_cleanup(curl);

	return response;
}
