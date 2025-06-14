#include <iostream>
#include <limits>
#include <vector>
#include <iomanip>
#include "HttpClient.hpp"
#include "Request.hpp"
#include "RequestLoader.hpp"
#include "Spinner.hpp"

static constexpr char ANSI_RESET[] = "\x1b[0m";
static constexpr char ANSI_GREEN[] = "\x1b[32m";
static constexpr char ANSI_RED[] = "\x1b[31m";
static constexpr int UNDER_BAR_SIZE = 45;

void printStatus(int code) {
    // 色を選択
    const char* color = (code == 200 ? ANSI_GREEN : ANSI_RED);

    // 幅揃えして出力
    std::cout
        << color << std::left << std::setw(8)
        << code
        << ANSI_RESET;
}

int main(int argc, char* argv[]) {
    std::cout << "cue-api-runner 起動OK\n";
    const std::vector<Request> requestList =
        RequestLoader::loadFromYaml("../../../config/requests.yaml");
    std::cout << "合計 " << requestList.size() << "件のリクエストを読み込みました\n\n";

    Spinner spinner("|/-\\", 100);

    // ヘッダ表示（オプション）
    std::cout
        << std::left << std::setw(22) << " Name"
        << std::left << std::setw(8) << "Status"
        << std::left << std::setw(12) << "Time (ms)"
        << "\n";
    std::cout << std::string(UNDER_BAR_SIZE, '-') << "\n";

    for (const Request& request : requestList) {
        // 実行開始
        std::cout
            << " " << std::left << std::setw(13) << request.getName() << " を実行中...";
        spinner.start();

        HttpResponse response;
        try {
            response = HttpClient::perform(request);
        }
        catch (const std::exception& e) {
            spinner.stop();
            std::cerr << "\nエラーが発生しました: " << e.what() << "\n";
            continue;
        }

        spinner.stop();

        // 出力
        double ms = response.totalTime * 1000.0;
        std::cout << "\r";
        std::cout
            << std::left << std::setw(22) << request.getName();
        printStatus(response.statusCode);
        std::cout
            << std::fixed << std::setprecision(1)
            << std::right << std::setw(7) << ms << " ms\n";
    }
    std::cout << std::string(UNDER_BAR_SIZE, '-') << "\n\n";
    std::cout << "全てのリクエストの実行が完了しました。終了するには Enter キーを押してください…" << "\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return 0;
}