//
// Created by yasas on 4/29/24.
//

#pragma once


#include <string>
#include <unordered_map>
#include <any>
#include "../Constants/events.h"
#include <stdexcept>

namespace Basics {

    class BaseTraveller{

    public:
        explicit BaseTraveller(Constants::Events event):_event(event){}
        template<typename T>
        BaseTraveller* add_data(const std::string& key, const T& data){
            _data_map[key].emplace_back(std::move(data));
            return this;
        }

        template<typename T>
        const std::vector<T>& get_data(const std::string& key){
            auto it = _data_map.find(key);
            if (it != _data_map.end()) {
                return it->second;
            }
            throw std::out_of_range("Key not found in the data map.");
        }

        void remove_data(const std::string& key){

            auto it = _data_map.find(key);
            if(it != _data_map.end()){
                _data_map.erase(key);
            }

            throw std::out_of_range("Key not found in the data map.");

        }

        template<typename T>
        std::vector<T> get_typed_vector(const std::string& key) {
            auto it = _data_map.find(key);
            if (it == _data_map.end()) {
                throw std::out_of_range("Key not found in the data map.");
            }

            std::vector<T> typed_vector;
            for (auto& any_value : it->second) {
                try {
                    typed_vector.push_back(std::any_cast<T>(any_value));
                }
                catch (const std::bad_any_cast& e) {
                    throw std::runtime_error("Type mismatch when attempting to cast the elements.");
                }
            }
            return typed_vector;
        }

        [[nodiscard]] const Constants::Events& get_event() const {
            return _event;
        }

        void set_error_code(int32_t error_code){_error_code = error_code;}
        int32_t get_error_code(){return _error_code;}

        void set_error_msg(const std::string& error_message) {_error_message = error_message ;}
        std::string get_error_message(){return  _error_message ;}

        virtual ~BaseTraveller()= default;

    protected:
        std::unordered_map<std::string ,std::vector<std::any>> _data_map;
        Constants::Events _event;
        int32_t _error_code = 200;
        std::string _error_message ;
    };
}

