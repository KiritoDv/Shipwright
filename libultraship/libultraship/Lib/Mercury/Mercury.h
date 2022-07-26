#pragma once
#include <any>
#include <vector>
#include <string>
#include "../nlohmann/json.hpp"

class Mercury {
protected:
    std::string path_;
    std::unordered_map<std::string, std::any> ramMap;
public:
    explicit Mercury(std::string path);

    nlohmann::json vjson;
    nlohmann::json rjson;
    nlohmann::json nested(const std::string& key);
    static std::string formatNestedKey(const std::string& key);
	
    bool contains(const std::string& key);
    void erase(const std::string& key);
	

    template< typename T > void setArray(const std::string& key, std::vector<T> arr);
    template< typename T > std::vector<T> getArray(const std::string& key);
	
    template< typename T > 
    void set(const std::string& key, T value) {
        this->ramMap[key] = value;
    };
	
    template< typename T >    
    T get(const std::string& key, T def) {
        return this->ramMap.contains(key) ? std::any_cast<T>(this->ramMap[key]) : def;
    };

    void reload();
    void save();
    bool isNewInstance = false;
};

template <typename T>
void Mercury::setArray(const std::string& key, std::vector<T> arr) {
    this->ramMap[key] = nlohmann::json(arr);    
}

template< typename T >
std::vector<T> Mercury::getArray(const std::string& key) {
    if (nlohmann::json tmp = this->nested(key); tmp.is_array())
        return tmp.get<std::vector<T>>();
    return std::vector<T>();
};