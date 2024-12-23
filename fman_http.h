#ifndef FMAN_HTTP_H
#define FMAN_HTTP_H

#include <iostream>
#include <curl/curl.h>
#include <thread>

namespace fman {

    struct HttpResponse {
        std::optional<int> code = std::nullopt;
        std::optional<std::string> data = std::nullopt;
        float time_took = 0.f;
    };

    class CurlClient {
    public:
        void Get(std::string &url, fman::HttpResponse &response) noexcept;

        CurlClient();
        ~CurlClient();

        /**
         * Singletons should not be cloneable, assignable
         */
        CurlClient(CurlClient &other) = delete;
        void operator=(const CurlClient &) = delete;

        static CurlClient *GetInstance();

    private:

        static CurlClient * _pinstance;
        static std::mutex _mutex;

        std::mutex _curl_mutex;
        std::vector<std::thread*> _threads;

        CURL* _curl;
        void _get(const char *url, long &code, std::string &read_buffer);
        static size_t _write_callback(void* contents, size_t size, size_t nmemb, std::string* userData);
    };
}

#endif //FMAN_HTTP_H
