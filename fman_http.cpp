#include "fman_http.h"

void fman::CurlClient::Get(std::string &url) {
    CURLcode res;
    std::string readBuffer;

    curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _write_callback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(_curl);

    if (res != CURLE_OK) {
        std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "Response:\n" << readBuffer << std::endl;
    }
}


fman::CurlClient *fman::CurlClient::GetInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new fman::CurlClient();
    }
    return pinstance_;
}

size_t fman::CurlClient::_write_callback(void *contents, size_t size, size_t nmemb, std::string *userData) {
    size_t totalSize = size * nmemb;
    userData->append((char*)contents, totalSize);
    return totalSize;
}

fman::CurlClient::CurlClient() {
    _curl = curl_easy_init();
}

fman::CurlClient::~CurlClient() {
    curl_easy_cleanup(_curl);
}

fman::CurlClient* fman::CurlClient::pinstance_{nullptr};
std::mutex fman::CurlClient::mutex_;

