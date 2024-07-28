#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <curl/curl.h>
#include <nlohmann/json.hpp> // Make sure this library is included and set up

std::string sharedTimeData; // Global variable to store the shared time data

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
            std::cout << "Fetched current time: " << jsonResponse["datetime"] << std::endl;
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
    std::ofstream file("timeDataM.txt", std::ios::app);
    if (file.is_open()) {
        file << data << std::endl;
        file.close();
        std::cout << "Logged time data to file: " << data << std::endl;
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}

// Function to check the number of lines in the file
int getNumberOfLinesInFile() {
    std::ifstream file("timeDataM.txt");
    int lines = 0;
    std::string line;
    while (std::getline(file, line)) {
        ++lines;
    }
    std::cout << "Number of lines in file: " << lines << std::endl;
    return lines;
}

void updateAndDisplayTimeData() {
    while (getNumberOfLinesInFile() < 5) {
        std::string currentTime = fetchCurrentTime();

        // Update sharedTimeData
        sharedTimeData = currentTime;
        std::cout << "Updated sharedTimeData: " << currentTime << std::endl;

        writeTimeDataToFile(currentTime);

        // Display sharedTimeData
        if (!sharedTimeData.empty()) {
            std::cout << "Current time is: " << sharedTimeData << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(6));
    }
}

int main() {
    std::cout << "Starting time updater and displayer..." << std::endl;
    updateAndDisplayTimeData();
    std::cout << "Time updater and displayer has finished." << std::endl;
    return 0;
}

