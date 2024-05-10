//
// Created by yasas on 4/28/24.
//

#pragma once


#include <string>
#include <nlohmann/json.hpp>
#include <iostream>
#include <utility>

class Student {
public:
    Student(int id, const std::string& student_name, const std::string& email, int age, const std::string& stream) : _id(id), _student_name(student_name),
                                                                            _email(email) , _age(age) , _stream(stream){}
    explicit Student(const nlohmann::json& j) {
        _student_name = j["name"].get<std::string>();
        _age = j["age"].get<int>();
        _email = j["email"].get<std::string>();
        _stream = j["stream"].get<std::string>();

        if (j.contains("id")) {
            _id = j["id"].get<int>();  // Only get id if it's present
        } else {
            _id = -1;  // Assign a default value if 'id' is not present
        }
    }

    [[nodiscard]] std::string toJSON() const {
        return to_string(nlohmann::json{{"id", _id},{"name", _student_name}, {"age", _age},{"email", _email} , {"stream",_stream}});
    }


    int _id{} ;
    std::string _student_name;
    std::string _email ;
    int _age;
    std::string _stream;
};

