//
// Created by yasas on 5/1/24.
//

#pragma once
#include "../EventBus/IEventListener.h"
#include "../Basics/BaseTraveller.h"
#include "DataFinalizeHandlers/BaseResponseMapperHandler.h"

namespace Finalizer{

class DataFinalizer : public EventBus::IEventListener<Basics::BaseTraveller>{

public:
    DataFinalizer();
    void listen(Basics::BaseTraveller& data) override;

private:
    std::unordered_map<Constants::Events, ResponseMapHandlers::BaseResponseMapHandler*> _response_mappers;

};


}

