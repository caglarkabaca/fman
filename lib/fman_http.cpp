#include "fman_http.h"

#include <chrono>

void fman::CurlClient::Get(std::string &url, fman::HttpResponse &response) noexcept {
    _threads.push_back(
        new std::thread([this, &url, &response]() {
            std::string _response;
            long code = 0;
            auto start = std::chrono::high_resolution_clock::now();
            this->_get(url.c_str(), code, _response);
            auto time_took = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

            response.code = code;
            response.data = _response;
            response.time_took = time_took.count();
        }
    ));
}


fman::CurlClient *fman::CurlClient::GetInstance() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_pinstance == nullptr)
    {
        _pinstance = new fman::CurlClient();
    }
    return _pinstance;
}

void fman::CurlClient::_get(const char* url, long& code, std::string& read_buffer) {
    std::lock_guard<std::mutex> lock(_curl_mutex);

    curl_easy_setopt(_curl, CURLOPT_URL, url);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _write_callback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &read_buffer);

    curl_easy_perform(_curl);
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &code);
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

fman::CurlClient* fman::CurlClient::_pinstance{nullptr};
std::mutex fman::CurlClient::_mutex;

