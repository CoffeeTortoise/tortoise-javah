#include <iostream>
#include <fstream>
#include <cctype>
#include "../include/Header.hpp"
#include "../include/Constants.hpp"


namespace tortoise_javah {

    Header::Header(std::string& name) {
        for (char c : name) this->fullname.push_back(c);
        int i = static_cast<int>(name.size()) - 1;
        for (; name.at(i) == ' '; i--);
        std::string revName;
        for (; name.at(i) != '/'; i--) {
            revName.push_back(name.at(i));
        }
        i = static_cast<int>(revName.size()) - 1;
        for (; i >= 0; i--) {
            this->name.push_back(revName.at(i));
        }
    }

    int Header::generateJAVAH(std::string& outDir) {
        int ind = static_cast<int>(this->name.size()) - 1;
        std::string fileExt;
        for (; this->name.at(ind) != '.'; ind--) {
            fileExt.push_back(this->name.at(ind));
        }
        if (fileExt.compare("avaj\0")) {
            return EXIT_ERR_EXT;
        }
        std::ifstream file(this->fullname);
        if (file.is_open()) {
            char c;
            std::string line;
            std::string packName, className, prefix;
            bool prefixCreated = false;
            while (file.get(c)) {
                if (c != ';' && c != '}' && c != '{') {
                    line.push_back(c);
                }
                else {
                    if (line.find(IGNORE_MARK) != std::string::npos) {
                        for (Function* func : this->functions) {
                            if (func) {
                                delete func;
                                func = nullptr;
                            }
                        }
                        return EXIT_OK_NOT_FOR_JNI;
                    }
                    int type = Header::checkLine(line);
                    if (type == PACKAGE_NAME && packName.empty()) {
                        packName = Header::transformPackName(line);
                    }
                    if (type == CLASS_NAME && className.empty()) {
                        className = Header::transformClassName(line);
                    }
                    if (type == FUNC_NAME) {
                        this->transformFuncName(line);
                    }
                    if (!className.empty() && !packName.empty() && !prefixCreated) {
                        prefix = "Java_" + packName + "_" + className + "_";
                        prefixCreated = true;
                    }
                    line.clear();
                }
            }
            file.close();
            int r = this->makeHeaderFile(prefix, outDir);
            return r;
        }
        else {
            std::cerr << "Couldn\'t open file!\n";
            return EXIT_ERR;
        }
    }

    int Header::makeHeaderFile(std::string& funPrefix, std::string& outDir) {
        std::string authorInfo = "//Generated with tortoise-javah(author: CoffeeTortoise(e.8ychkov@yandex.ru))";
        std::string macro = this->genHeaderMacro();
        std::string beginFile = authorInfo + "\n\n" + "#ifndef " + macro + "\n#define " + macro;
        beginFile += "\n\n#include<jni.h>\n\n\n#ifdef __cplusplus\nextern \"C\" {\n#endif";
        std::string endFile = "#ifdef __cplusplus\n}\n#endif\n\n#endif";
        std::string func = this->genHeaderContent(funPrefix);
        std::string content = beginFile + "\n\n" + func + endFile;
        std::string fileName = outDir + "/" + this->genHeaderName();
        for (Function* f : this->functions) {
            if (f) {
                delete f;
                f = nullptr;
            }
        }
        std::ofstream file(fileName, std::ios::out);
        if (file.is_open()) {
            for (char c : content) {
                file << c;
            }
            file.close();
            return EXIT_OK;
        }
        std::cerr << "Failed to create header!\n";
        return EXIT_ERR;
    }

    std::string Header::genHeaderMacro() {
        std::string macro;
        macro.append("JNI_");
        for (char c : this->name) {
            if (c == '.') break;
            macro.push_back(
                std::toupper(static_cast<unsigned char>(c))
            );
        }
        macro.append("_H");
        return macro;
    }

    std::string Header::genHeaderName() {
        std::string name;
        name.append("JNI_");
        for (char c : this->name) {
            if (c == '.') break;
            name.push_back(c);
        }
        name.append(".h");
        return name;
    }

    std::string Header::genHeaderContent(std::string& prefix) {
        std::string contFunc;
        for (Function* f : this->functions) {
            std::string func = f->genFunc(prefix);
            contFunc.append(func);
            contFunc.append(";\n\n");
        }
        return contFunc;
    }

    void Header::transformFuncName(std::string& funcName) {
        bool isStatic = funcName.find("static ") != std::string::npos;
        int i = static_cast<int>(funcName.size()) - 1;
        for (; funcName.at(i) == ' '; i--);
        std::string revArgs;
        for (; funcName.at(i) != '('; i--) {
            revArgs.push_back(funcName.at(i));
        }
        revArgs.push_back(funcName.at(i));
        i--;
        for (; funcName.at(i) == ' '; i--);
        std::string revName;
        for (; funcName.at(i) != ' '; i--) {
            revName.push_back(funcName.at(i));
        }
        for (; funcName.at(i) == ' '; i--);
        std::string revType;
        for (; funcName.at(i) != ' '; i--) {
            revType.push_back(funcName.at(i));
        }
        static auto rev = [](std::string& str) -> std::string {
            int ind = static_cast<int>(str.size()) - 1;
            std::string revStr;
            for (; ind >= 0; ind--) {
                revStr.push_back(str.at(ind));
            }
            return revStr;
        };
        std::string name = rev(revName), args = rev(revArgs), retType = rev(revType);
        this->functions.push_back(
            new Function(isStatic, name, retType, args)
        );
    }

    std::string Header::transformClassName(std::string& className) {
        std::string name;
        int i = static_cast<int>(className.size()) - 1;
        for (; className.at(i) == ' '; i--);
        std::string buffer;
        for(; buffer.find(" ssalc") == std::string::npos; i--) {
            buffer.push_back(className.at(i));
        }
        i = static_cast<int>(buffer.size()) - 1;
        for (; buffer.at(i) != ' '; i--);
        for (int j = i - 1; buffer.at(j) != ' '; j--) {
            name.push_back(buffer.at(j));
        }
        return name;
    }

    std::string Header::transformPackName(std::string& packName) {
        std::string revName;
        int i = static_cast<int>(packName.size()) - 1;
        for (; packName.at(i) == ' '; i--);
        for (; packName.at(i) != ' '; i--) {
            revName.push_back(packName.at(i));
        }
        std::string name;
        i = static_cast<int>(revName.size()) - 1;
        for (; i >= 0; i--) {
            char c = revName.at(i);
            if (c == '.') {
                c = '_';
            }
            name.push_back(c);
        }
        return name;
    }

    int Header::checkLine(std::string& line) {
        if (line.find("package") != std::string::npos) return PACKAGE_NAME;
        if (line.find("class") != std::string::npos) return CLASS_NAME; 
        if (line.find("native") != std::string::npos) return FUNC_NAME;
        return TRASH;
    }

}