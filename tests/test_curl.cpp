#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fman_http.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

TEST(CurlClient, CurlTest) {
    fman::CurlClient *client = fman::CurlClient::GetInstance();

    fman::HttpResponse response;
    std::string get_url = "https://httpbin.org/get";
    client->Get(get_url, response);
    while (!response.code.has_value()) {} // wait to get response
    ASSERT_EQ(response.code.value(), 200);
}