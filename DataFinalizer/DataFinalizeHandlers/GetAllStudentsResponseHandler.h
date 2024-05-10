//
// Created by yasas on 5/1/24.
//

#pragma once

#include "../../Basics/BaseTraveller.h"
#include "BaseResponseMapperHandler.h"
#include "../../Models/Student.h"

namespace Finalizer:: ResponseMapHandlers{

    class GetAllStudentsResponseHandler: public BaseResponseMapHandler{
        using json = nlohmann::json;
    public:
        std::string handel_response(Basics::BaseTraveller& data_traveller) override {

            const std::vector<Student*>& students = data_traveller.get_typed_vector<Student*>("response_data");
            json jsonResponse = json::array();
            for(auto& student : students){
                json studentJson = json::parse(student->toJSON());
                jsonResponse.push_back(studentJson);
            }
            return jsonResponse.dump();
        }
    };

}
