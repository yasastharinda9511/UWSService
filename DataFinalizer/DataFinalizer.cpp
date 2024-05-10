//
// Created by yasas on 5/1/24.
//

#include "DataFinalizer.h"
#include "../Basics/DataTraveller.h"
#include "DataFinalizeHandlers/AddStudentResponseHandler.h"
#include "DataFinalizeHandlers/ErrorHandler.h"
#include "DataFinalizeHandlers/GetAllStudentsResponseHandler.h"

void Finalizer::DataFinalizer::listen(Basics::BaseTraveller &base_data_traveller) {
    auto data_traveller = dynamic_cast<Basics::DataTraveller<false>*>(&base_data_traveller);

    if(data_traveller != nullptr){
        auto ite = _response_mappers.find(base_data_traveller.get_event());
        if(base_data_traveller.get_error_code() != 200) {
            std::string response_json = _response_mappers.find(Constants::Events::ERROR)->second->handel_response(base_data_traveller);
            data_traveller->get_response()->end(response_json);
            data_traveller->get_response()->writeHeader("Connection", "keep-alive");
            delete data_traveller;
            return;
        }

        if(ite != _response_mappers.end()){
            std::string response_json = ite->second->handel_response(base_data_traveller);
            data_traveller->get_response()->writeHeader("Connection", "keep-alive");
            data_traveller->get_response()->end(response_json);
        }

        delete data_traveller;
    }
}

Finalizer::DataFinalizer::DataFinalizer() {

    _response_mappers.try_emplace(Constants::Events::ADD_STUDENT, new Finalizer::ResponseMapHandlers::AddStudentResponseHandler());
    _response_mappers.try_emplace(Constants::Events::GET_ALL_STUDENTS, new Finalizer::ResponseMapHandlers::GetAllStudentsResponseHandler());
    _response_mappers.try_emplace(Constants::Events::ERROR, new Finalizer::ResponseMapHandlers::ErrorHandler());
}
