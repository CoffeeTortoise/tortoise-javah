#include <filesystem>
#include <iostream>
#include "../include/TortoiseJAVAH.hpp"
#include "../include/Constants.hpp"
#include "../include/Header.hpp"

namespace fs = std::filesystem;


namespace tortoise_javah {

    int TortoiseJAVAH::genFromDirAndSub() {
        for (const auto& entry : fs::recursive_directory_iterator(this->inputDir)) {
            std::string filename = entry.path().string();
            int i = static_cast<int>(filename.size()) - 1;
            std::string end;
            for (; filename.at(i) != '.'; i--) {
                char c = filename.at(i);
                end.push_back(c);
            }
            if (!end.compare("avaj")) {
                std::replace(filename.begin(), filename.end(), '\\', '/');
                Header header(filename);
                int r = EXIT_OK;
                if (this->sameDirs) {
                    int j = static_cast<int>(filename.size()) - 1;
                    for (; filename.at(j) != '/'; j--);
                    std::string outDir;
                    for (int k = 0; k < j; k++) {
                        outDir.push_back(filename.at(k));
                    }
                    r = header.generateJAVAH(outDir);
                }
                else {
                    r = header.generateJAVAH(this->outDir);
                }
                if (r != EXIT_OK && r != EXIT_OK_NOT_FOR_JNI) {
                    return r;
                }
            }
        }
        return EXIT_OK;
    }

    int TortoiseJAVAH::genFromDir() {
        for (const auto& entry : fs::directory_iterator(this->inputDir)) {
            std::string filename = entry.path().string();
            int i = static_cast<int>(filename.size()) - 1;
            std::string end;
            for (; filename.at(i) != '.'; i--) {
                char c = filename.at(i);
                end.push_back(c);
            }
            if (!end.compare("avaj")) {
                std::replace(filename.begin(), filename.end(), '\\', '/');
                Header header(filename);
                int r = header.generateJAVAH(this->outDir);
                if (r != EXIT_OK && r != EXIT_OK_NOT_FOR_JNI) {
                    return r;
                }
            }
        }
        return EXIT_OK;
    }

    int TortoiseJAVAH::genOne(std::string& filename) {
        std::string fullPath = this->inputDir + "/" + filename;
        if (fs::exists(fullPath)) {
            if (fs::is_regular_file(fullPath)) {
                Header header(fullPath);
                int r = header.generateJAVAH(this->outDir);
                return r;
            }
            return EXIT_ERR_NOT_A_FILE;
        }
        return EXIT_ERR_PATH_NOT_EXISTS;
    }

    int TortoiseJAVAH::setOutDir(std::string& outDir) {
        std::replace(outDir.begin(), outDir.end(), '\\', '/');
        outDir.erase(
            std::remove(outDir.begin(), outDir.end(), '\''), outDir.end()
        );
        outDir.erase(
            std::remove(outDir.begin(), outDir.end(), '\"'), outDir.end()
        );
        if (fs::exists(outDir)) {
            if (fs::is_directory(outDir)) {
                if (!this->outDir.empty()) this->outDir.clear();
                for (char c : outDir) this->outDir.push_back(c);
                return EXIT_OK;
            }
            return EXIT_ERR_NOT_A_DIRECTORY;
        }
        if (fs::create_directory(outDir)) {
            return EXIT_ERR_MADE_DIR;
        }
        return EXIT_ERR_PATH_NOT_EXISTS;
    }

    int TortoiseJAVAH::setInputDir(std::string& inputDir) {
        std::replace(inputDir.begin(), inputDir.end(), '\\', '/');
        inputDir.erase(
            std::remove(inputDir.begin(), inputDir.end(), '\''), inputDir.end()
        );
        inputDir.erase(
            std::remove(inputDir.begin(), inputDir.end(), '\"'), inputDir.end()
        );
        std::cout << "Got: " << inputDir << "\n";
        if (fs::exists(inputDir)) {
            if (fs::is_directory(inputDir)) {
                if (!this->inputDir.empty()) this->inputDir.clear();
                for (char c : inputDir) this->inputDir.push_back(c);
                return EXIT_OK;
            }
            return EXIT_ERR_NOT_A_DIRECTORY;
        }
        return EXIT_ERR_PATH_NOT_EXISTS;
    }

    void TortoiseJAVAH::setSameDirs(bool sameDirs) {
        this->sameDirs = sameDirs;
    }

    std::string TortoiseJAVAH::getInputDir() {
        return this->inputDir;
    }

}