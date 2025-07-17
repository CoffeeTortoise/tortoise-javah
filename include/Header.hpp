#pragma once

#include "../include/Function.hpp"


namespace tortoise_javah {

    static const std::string IGNORE_MARK = "NOT FOR JNI";

    
    class Header {

        std::string name;

        std::string fullname;

        std::vector<Function*> functions;

        enum {FUNC_NAME, CLASS_NAME, PACKAGE_NAME, TRASH};

        static int checkLine(std::string& line);

        static std::string transformPackName(std::string& packName);

        static std::string transformClassName(std::string& className);

        std::string genHeaderContent(std::string& prefix);

        std::string genHeaderName();

        std::string genHeaderMacro();

        void transformFuncName(std::string& funcName);

        int makeHeaderFile(std::string& funPrefix, std::string& outDir);

        public:

        Header(std::string& name);

        int generateJAVAH(std::string& outDir);

    };

}