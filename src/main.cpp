#include <iostream>
#include <vector>
#include "Request.hpp"
#include "RequestLoader.hpp"

int main(int argc, char* argv[]) {
  std::cout << "cue-api-runner 起動OK" << std::endl;
  const std::vector<Request> requestList = RequestLoader::loadFromYaml("../../../config/requests.yaml");
  std::cout << "合計 " << requestList.size() << "件のリクエストを読み込みました" << std::endl;

  return 0;
}