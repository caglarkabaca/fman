#include "fman_http.h"

#include <chrono>

void fman::CurlClient::Request(fman::HttpRequest &request, fman::HttpResponse &response) noexcept {
    _threads.push_back(
        new std::thread([this, &request, &response]() {
            this->_request(request, response);
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

void fman::CurlClient::_request(HttpRequest &req, HttpResponse &resp) {
    std::lock_guard<std::mutex> lock(_curl_mutex);

    std::string buffer;
    long code;

    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, _write_callback);

    curl_easy_setopt(_curl, CURLOPT_URL, req.url.c_str());

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);

    if (req.body.size() > 0) {
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, req.body.c_str());
    }

    switch (req.method) {
        case GET:
            curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "GET");
        break;
        case POST:
            curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "POST");
        break;
        default: break;
    }

    std::cout << "Sending " << req.method << " request to " << req.url << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    curl_easy_perform(_curl);
    auto time_took = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
    std::cout << "Got response!" << std::endl;

    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &code);

    resp.data = buffer;
    resp.code = code;
    resp.time_took = time_took.count();
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

