#pragma once

#include <string>
#include <vector>


namespace tortoise_javah {

    class Function {

        std::string name;

        std::string return_type;

        std::string args;

        std::string transformTypes(std::string& type);

        std::string transformArgs(std::string& args, bool isStatic);

        std::string convVariable(std::string& var);

        std::string createFuncArgs(std::vector<std::string>& vars);
        
        public:

        Function(
            bool isStatic, 
            std::string name, 
            std::string& return_type, 
            std::string& args
        );

        std::string genFunc(std::string& prefix);

    };

}