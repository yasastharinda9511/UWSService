//
// Created by yasas on 5/1/24.
//

#pragma once

#include "../../Basics/BaseTraveller.h"
#include "BaseResponseMapperHandler.h"
#include "../../Models/Student.h"

namespace Finalizer:: ResponseMapHandlers{

    class AddStudentResponseHandler: public BaseResponseMapHandler{
    public:
        std::string handel_response(Basics::BaseTraveller& data_traveller) override {

            const std::vector<Student>& students = data_traveller.get_typed_vector<Student>("response_data");
            std::string json_response = "" ;
            for(auto& student : students){

                std::cout << student.toJSON() << std::endl;
                json_response +=student.toJSON();
            }
            return json_response;
        }
    };

}
