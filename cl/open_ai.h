#ifndef OPEN_AI_H
#define OPEN_AI_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <curl/curl.h>
#include </usr/include/jsoncpp/json/json.h>
#include <sstream>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string getApiKeyFromEnv() {
    const char* apiKey = std::getenv("OPENAI_API_KEY");
    if (apiKey == nullptr) {
        std::cerr << "Error: API key not set in environment variables." << std::endl;
        exit(1);
    }
    return std::string(apiKey);
}

std::string makeOpenAIRequest(const std::string& prompt, const std::string& apiKey, const std::string& model) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        Json::Value root;
        root["model"] = model;
        Json::Value messages(Json::arrayValue);

        Json::Value userMessage;
        userMessage["role"] = "user";
        userMessage["content"] = prompt;
        messages.append(userMessage);
        root["messages"] = messages;

        Json::StreamWriterBuilder writer;
        std::string payload = Json::writeString(writer, root);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str()); // c_str add '\0' : string -> char*

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    return readBuffer;
}

#endif // OPEN_AI_H
