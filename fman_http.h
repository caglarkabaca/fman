#ifndef FMAN_HTTP_H
#define FMAN_HTTP_H

#include <iostream>
#include <curl/curl.h>
#include <mutex>

namespace fman {

    class CurlClient {
    public:
        void Get(std::string &url);

        CurlClient();
        ~CurlClient();

        /**
         * Singletons should not be cloneable, assignable
         */
        CurlClient(CurlClient &other) = delete;
        void operator=(const CurlClient &) = delete;

        static CurlClient *GetInstance();

    private:

        static CurlClient * pinstance_;
        static std::mutex mutex_;

        CURL* _curl;
        static size_t _write_callback(void* contents, size_t size, size_t nmemb, std::string* userData);
    };

}

#endif //FMAN_HTTP_H
