#include <iostream>
#include "include/TortoiseJAVAH.hpp"
#include "include/Constants.hpp"
#include "include/Header.hpp"


enum {
    JAVAH_ONE_FILE,
    JAVAH_DIRECTORY,
    JAVAH_DIRECTORY_AND_SUBS,
    JAVAH_INSTRUCTION,
    JAVAH_EXIT,
    JAVAH_CONTINUE
};


void showInstruction();

void getInputDir(tortoise_javah::TortoiseJAVAH& tortoise);

void getOutDir(tortoise_javah::TortoiseJAVAH& tortoise);

void readString(std::string& str);

int getChoice(tortoise_javah::TortoiseJAVAH& tortoise);


int main() {
    #ifdef _WIN32
    system("chcp 65001");
    #endif

    tortoise_javah::TortoiseJAVAH tortoise;
    bool run = true;
    showInstruction();
    while (run) {
        int choice = getChoice(tortoise);
        if (choice > JAVAH_EXIT || choice < JAVAH_ONE_FILE) {
            std::cout << "Incorrect option!\nTry again.\n";
            continue;
        }
        if (choice == JAVAH_EXIT) {
            run = false;
            break;
        }
        getInputDir(tortoise);
        getOutDir(tortoise);
        std::string filename;
        int r;
        switch (choice) {

            case JAVAH_ONE_FILE:
            std::cout << "Enter filename:\n\t";
            readString(filename);
            r = tortoise.genOne(filename);
            filename.clear();
            if (r == tortoise_javah::EXIT_ERR_NOT_A_FILE) {
                std::string full = tortoise.getInputDir() + "/" + filename;
                std::cout << full << " is not a file!\n";
            }
            break;

            case JAVAH_DIRECTORY:
            r = tortoise.genFromDir();
            break;

            case JAVAH_DIRECTORY_AND_SUBS:
            r = tortoise.genFromDirAndSub();
            break;

            case JAVAH_INSTRUCTION:
            showInstruction();
            r = JAVAH_CONTINUE;
            break;

            case JAVAH_EXIT:
            run = false;
            r = JAVAH_EXIT;
            break;

        }

        if (r == tortoise_javah::EXIT_ERR) {
            std::cout << "Found an error!\n";
        }
        if (r == JAVAH_EXIT) {
            run = false;
        }

    }

    #ifdef _WIN32
    system("pause");
    #else
    system("read");
    #endif
    return 0;
}

int getChoice(tortoise_javah::TortoiseJAVAH& tortoise) {
    std::cout << "Available options:\n";
    std::cout << "\tGenerate jni header for one file: " << JAVAH_ONE_FILE << "\n";
    std::cout << "\tGenerate jni headers for files in one directory: ";
    std::cout << JAVAH_DIRECTORY << "\n";
    std::cout << "\tGenerate jni headers for files in directory and it\'s subdirectories: ";
    std::cout << JAVAH_DIRECTORY_AND_SUBS << "\n";
    std::cout << "\tGet information about tortoise-javah: " << JAVAH_INSTRUCTION << "\n";
    std::cout << "\tExit an app: " << JAVAH_EXIT << "\n";
    int choice;
    std::cout << "Enter an option:\n\t";
    std::cin >> choice;
    std::cin.get();
    return choice;
}

void getOutDir(tortoise_javah::TortoiseJAVAH& tortoise) {
    std::cout << "Is input and output directories are same?(Enter 0 if not or 1 if yes)\n\t";
    int a;
    std::cin >> a;
    std::cin.get();
    if (a) {
        tortoise.setSameDirs(true);
        tortoise.setOutDir(tortoise.getInputDir());
    }
    else {
        bool run = true;
        while (run) {
            std::cout << "Please enter output directory:\n\t";
            std::string outDir;
            readString(outDir);
            int r = tortoise.setOutDir(outDir);
            if (r == tortoise_javah::EXIT_ERR_NOT_A_DIRECTORY) {
                std::cout << outDir << " is not a directory!\nTry again.\n";
            }
            else if (r == tortoise_javah::EXIT_ERR_MADE_DIR) {
                std::cout << "Made directory " << outDir << " because it didn\'t existed.\n";
                run = false;
            }
            else if (r == tortoise_javah::EXIT_ERR_PATH_NOT_EXISTS) {
                std::cout << "Path doesn\'t exists!\nTry again.\n";
            }
            else {
                run = false;
            }
        }
    }
}

void getInputDir(tortoise_javah::TortoiseJAVAH& tortoise) {
    bool run = true;
    while(run) {
        std::cout << "Please enter input directory:\n\t";
        std::string inputDir;
        readString(inputDir);
        int r = tortoise.setInputDir(inputDir);
        if (r == tortoise_javah::EXIT_ERR_NOT_A_DIRECTORY) {
            std::cout << inputDir << " is not a directory!\nTry again.\n";
        }
        else if (r == tortoise_javah::EXIT_ERR_PATH_NOT_EXISTS) {
            std::cout << "Path doesn\'t exists!\nTry again.\n";
        }
        else {
            run = false;
        }
    }
}

void readString(std::string& str) {
    int c;
    while((c = getchar()) != EOF && static_cast<char>(c) != '\n') {
        str.push_back(static_cast<char>(c));
    }
}

void showInstruction() {
    std::cout << "################################################################################\n";
    std::cout << "This is tortoise-javah, unofficial javah.\n";
    std::cout << "Created by CoffeeTortoise(e.8ychkov@yandex.ru). Date: 17.07.2025\n";
    std::cout << "Generates headers for java classes with native methods.\n";
    std::cout << "Java filename must ends with java.\n";
    std::cout << "Recommended format for the class name:\n";
    std::cout << "\tLike that: " << "public class AquaEvent extends AquaObject\n";
    std::cout << "The file must contain package name:\n";
    std::cout << "\tLike that: " << "package org.coffee_tortoise\n";
    std::cout << "Recommended format for native functions:\n";
    std::cout << "\tLike that: " << "private native int getEventType(long ptr)\n";
    std::cout << "Also string may be recognized as function if it contains \'native\' key word.\n";
    std::cout << "If you don\'t want generate jni header for a file, it must contain:\n";
    std::cout << "\t\'" << tortoise_javah::IGNORE_MARK << "\' string\n";
    std::cout << "It\'s better if the mark at the begining of the file.\n";
    std::cout << "Note that filename and filepath is not the same.\n";
    std::cout << "################################################################################\n";
}