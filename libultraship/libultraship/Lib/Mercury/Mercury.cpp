#include "Mercury.h"

#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <any>
#include <Utils/StringHelper.h>

namespace fs = std::filesystem;
using json = nlohmann::json;

Mercury::Mercury(std::string path) : path_(std::move(path)) {
	this->reload();
}

std::string Mercury::formatNestedKey(const std::string& key) {
	const std::vector<std::string> dots = StringHelper::Split(key, ".");
    std::string tmp;
    if (dots.size() > 1)
        for (const auto& dot : dots) {
            tmp += "/" + dot;
        }
    else
        tmp = "/" + dots[0];

    return tmp;
}

json Mercury::nested(const std::string& key) {
    std::vector<std::string> dots = StringHelper::Split(key, ".");
    if (!this->vjson.is_object())
        return this->vjson;
    json gjson = this->vjson.unflatten();

    if (dots.size() > 1) {
        for (auto& key : dots) {
            if (key == "*" || gjson.contains(key))
                gjson = gjson[key];
        }
        return gjson;
    }

    return gjson[key];
}

bool Mercury::contains(const std::string& key) {
    return !this->ramMap.contains(key);
}

void Mercury::erase(const std::string& key) {
    this->vjson.erase(formatNestedKey(key));
}

void Mercury::reload() {
    if (this->path_ == "None" || !fs::exists(this->path_) || !fs::is_regular_file(this->path_)) {
        this->isNewInstance = true;
        this->vjson = json::object();
        return;
    }
    std::ifstream ifs(this->path_);
    try {
        this->rjson = json::parse(ifs);
        this->vjson = this->rjson.flatten();
    }
    catch (...) {
        this->vjson = json::object();
    }
}

void Mercury::save() {
	
    for (auto& [key, val] : this->ramMap) {
        const std::string& vkey = formatNestedKey(key);
        if (val.type().name() == "string") {
            this->vjson[vkey] = std::any_cast<std::string>(val);
        } else if (val.type().name() == "int") {
            this->vjson[vkey] = std::any_cast<int>(val);
		} else if (val.type().name() == "double") {
            this->vjson[vkey] = std::any_cast<double>(val);
		} else if (val.type().name() == "bool") {
            this->vjson[vkey] = std::any_cast<bool>(val);
		} else if (val.type().name() == "float") {
            this->vjson[vkey] = std::any_cast<float>(val);
		}
    }
	
    std::ofstream file(this->path_);
    file << this->vjson.unflatten().dump(4);
}
