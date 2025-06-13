#include <iostream>
#include <vector>
#include "HttpClient.hpp"
#include "Request.hpp"
#include "RequestLoader.hpp"
#include "Spinner.hpp"

int main(int argc, char* argv[]) {
  std::cout << "cue-api-runner 起動OK" << std::endl;
  const std::vector<Request> requestList = RequestLoader::loadFromYaml("../../../config/requests.yaml");
  std::cout << "合計 " << requestList.size() << "件のリクエストを読み込みました" << std::endl;
  Spinner spinner("|/-\\", 100);

  for (const Request& request : requestList) {
	  std::cout << " [" << request.getName() << "]を実行中...";
	  spinner.start();
	  HttpResponse response;
	  try {
		  response = HttpClient::perform(request);
	  }
	  catch (const std::exception& e) {
		  spinner.stop();
		  std::cerr << "エラーが発生しました: " << e.what() << std::endl;
	  }
	  spinner.stop();
	  std::cout << std::endl;
	  std::cout << " ステータス: " << response.statusCode << std::endl;
	  std::cout << " ボディ長: " << response.body.size() << " バイト" << std::endl;
  }
  return 0;
}