#include <iostream>
#include <fstream>
#include <chrono>
#include <curl/curl.h>
#include <nlohmann/json.hpp> // Make sure this library is included and set up

std::string sharedTimeData;    // Global variable to store the shared time data

// This is a callback function used by libcurl for storing fetched data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to fetch the current time from an API
std::string fetchCurrentTime() {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://worldtimeapi.org/api/timezone/Japan");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        // Parse the response to extract the time
        try {
            auto jsonResponse = nlohmann::json::parse(readBuffer);
            return jsonResponse["datetime"];
        } catch(const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << '\n';
            return "";
        }
    }
    return "";
}

// Function to write the time data to a file
void writeTimeDataToFile(const std::string& data) {
    std::ofstream file("timeData.txt", std::ios::app);
    if (file.is_open()) {
        file << data << std::endl;
        file.close();
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}

// Function to check the number of lines in the file
int getNumberOfLinesInFile() {
    std::ifstream file("timeData.txt");
    int lines = 0;
    std::string line;
    while (std::getline(file, line)) {
        ++lines;
    }
    return lines;
}

// Function to simulate sleep without using <thread>
void sleepFor(int milliseconds) {
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(milliseconds)) {
        // Busy-wait loop
    }
}

int main() {
    using namespace std::chrono;
    
    auto nextUpdateTime = steady_clock::now();

    while (getNumberOfLinesInFile() < 5) {
        // Update time data if it's time to do so
        if (steady_clock::now() >= nextUpdateTime) {
            sharedTimeData = fetchCurrentTime();
            writeTimeDataToFile(sharedTimeData);
            nextUpdateTime = steady_clock::now() + seconds(6);  // Schedule the next update
        }

        // Display the time data
        if (!sharedTimeData.empty()) {
            std::cout << "Current time is: " << sharedTimeData << std::endl;
        }

        sleepFor(100);  // Busy-wait for 100 milliseconds
    }

    return 0;
}

