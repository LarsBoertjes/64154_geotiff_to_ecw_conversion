#include<iostream>
#include<filesystem>
#include<fstream>
#include<chrono>
#include<thread>
#include<set>
#include<gdal_priv.h>

namespace fs = std::filesystem;

void log(const std::string &message) {
    std::ofstream logFile("sample_data/logs/process.log", std::ios::app);
    auto t = std::time(nullptr);
    logFile << "[" << std::ctime(&t) << "] " << message << std::endl;
}

void convert_tif_to_ecw(const fs::path &inputPath) {
    std::string base = inputPath.stem().string();
    std::string outputPath = "sample_data/output/" + base + ".ecw";

    log("Converting " + inputPath.string() + " to " + outputPath);

    GDALAllRegister();
    GDALDataset = *src = (GDALDataset *)GDALOpen(inputPath.string().c_str(), GA_ReadOnly);
    if (!src) {
        log("Failed to open " + inputPath.string());
        return; 
    }

    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("ECW");
    if (!driver) {
        log("ECW driver not available.");
        GDALClose(src);
        return;
    }

    GDALDataset *dst = driver->CreateCopy(outputPath.c_str(), src, FALSE, nullptr, nullptr, nullptr);
    if (dst) {
        log("Successfully converted to " + outputPath);
        GDALClose(dst);
    } else {
        log("Failed to create ECW file for " + outputPath);
    }

    GDALClose(src);
}

int main() {
    fs::create_directories("sample_data/output");
    fs::create_directories("sample_data/logs");
    log("Starting watcher...");

    std::set<std::string> processed;
    while (true) {
        for (auto &entry : fs::directory_iterator("sample_data/input")) {
            if (entry.path().extension() == ".tif") {
                auto name = entry.path().filename().string();
                if (!processed.count(name)) {
                    convert_tif_to_ecw(entry.path());
                    processed.insert(name);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}