#include <cctype>
#include "../include/Function.hpp"


namespace tortoise_javah {

    Function::Function(
        bool isStatic, 
        std::string name, 
        std::string& return_type, 
        std::string& args
    ) {
        this->name = name;
        this->return_type = this->transformTypes(return_type);
        this->args = this->transformArgs(args, isStatic);
    }

    std::string Function::genFunc(std::string& prefix) {
        return "JNIEXPORT " + this->return_type + " JNICALL " + prefix + this->name + this->args;
    }

    std::string Function::transformArgs(std::string& args, bool isStatic) {
        std::string jargs = isStatic ? "JNIEnv* env, jclass clazz" : "JNIEnv* env, jobject obj";
        if (args.empty()) return "(" + jargs + ")";
        std::string arg;
        std::vector<std::string> ars;
        for (char c : args) {
            if (c == ',') {
                std::string a = this->convVariable(arg);
                arg.clear();
                ars.push_back(a);
            }
            if (c != '(' && c != ')') {
                arg.push_back(c);
            }
        }
        if (!arg.empty()) {
            std::string a = this->convVariable(arg);
            arg.clear();
            ars.push_back(a);
        }
        std::string arg_res = Function::createFuncArgs(ars);
        if (arg_res.empty() || arg_res == " ") {
            return "(" + jargs + ")";
        }
        return "(" + jargs + ", " + arg_res + ")";
    }

    std::string Function::createFuncArgs(std::vector<std::string>& vars) {
        if (vars.empty()) return "";
        size_t s = 0;
        for (std::string& str : vars) {
            s += str.size();
        }
        std::string res;
        res.reserve(s);
        int varsS = static_cast<int>(vars.size());
        for (int i = 0; i < varsS; i++) {
            res.append(vars[i]);
            if (i + 1 == varsS) break;
            res.append(", ");
        }
        return res;
    }

    std::string Function::convVariable(std::string& var) {
        std::string varType, varName;
        int i = 0;
        int argS = static_cast<int>(var.size());
        for (; i < argS; i++) {
            char k = var.at(i);
            if (k == ' ') break;
            varType.push_back(k);
        }
        for (; var.at(i) != ' '; i++);
        for (int j = i + 1; j < argS; j++) {
            char k = var.at(j);
            varName.push_back(k);
        }
        std::string ty = this->transformTypes(varType);
        return ty + " " + varName;
    }

    std::string Function::transformTypes(std::string& type) {
        std::string ty;
        for (char c : type) {
            ty.push_back(std::tolower(static_cast<unsigned char>(c)));
        }
        if (ty.compare("int\0") == 0) return "jint\0";
        if (ty.compare("long\0") == 0) return "jlong\0";
        if (ty.compare("float\0") == 0) return "jfloat\0";
        if (ty.compare("double\0") == 0) return "jdouble\0";
        if (ty.compare("int\0") == 0) return "jint\0";
        if (ty.compare("string\0") == 0) return "jstring\0";
        if (ty.compare("void\0") == 0) return "void\0";
        return "";
    }

}