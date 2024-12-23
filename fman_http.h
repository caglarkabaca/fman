//
// Created by Çağlar Kabaca on 23.12.2024.
//

#ifndef FMAN_HTTP_H
#define FMAN_HTTP_H

#include <iostream>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    size_t totalSize = size * nmemb;
    userData->append((char*)contents, totalSize);
    return totalSize;
}

CURL* _curl;

void HttpInit(void) {
    // cURL başlat
    _curl = curl_easy_init();
    if (!_curl) {
        std::cerr << "Failed to initialize cURL" << std::endl;
    }
}

void HttpGet(const std::string& url) {
    CURLcode res;
    std::string readBuffer;

    curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(_curl);

    // Hata kontrolü
    if (res != CURLE_OK) {
        std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "Response:\n" << readBuffer << std::endl;
    }
}

#endif //FMAN_HTTP_H
