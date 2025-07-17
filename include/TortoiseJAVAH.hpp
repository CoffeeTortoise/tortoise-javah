#pragma once

#include <string>


namespace tortoise_javah {

    class TortoiseJAVAH {

        std::string inputDir;

        std::string outDir;

        bool sameDirs;
        
        public:

        int genOne(std::string& filename);

        int genFromDir();

        int genFromDirAndSub();

        int setInputDir(std::string& inputDir);

        int setOutDir(std::string& outDir);

        void setSameDirs(bool sameDirs);

        std::string TortoiseJAVAH::getInputDir();

    };

}