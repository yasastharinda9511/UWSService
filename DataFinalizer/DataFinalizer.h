//
// Created by yasas on 5/1/24.
//

#pragma once
#include "../EventBus/IEventListener.h"
#include "../Basics/BaseTraveller.h"
#include "DataFinalizeHandlers/BaseResponseMapperHandler.h"
#include "uWebSockets/App.h"

namespace Finalizer{

class DataFinalizer : public EventBus::IEventListener<Basics::BaseTraveller>{

public:
    explicit DataFinalizer(uWS::Loop *loop);
    void listen(Basics::BaseTraveller& data) override;
    void set_event_loop(uWS::Loop *loop);

private:
    std::unordered_map<Constants::Events, ResponseMapHandlers::BaseResponseMapHandler*> _response_mappers;
    uWS::Loop *_loop;

};


}

