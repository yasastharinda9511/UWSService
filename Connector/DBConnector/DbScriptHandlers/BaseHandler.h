//
// Created by yasas on 4/29/24.
//

#pragma once

#include <pqxx/pqxx>
#include "../../../Basics/DataTraveller.h"
#include "../../../Stores/SqlStore.h"

namespace Connectors:: Handlers {

    class BaseHandler {
    public:
        virtual void handle(Basics::BaseTraveller& dataTraveller ,  pqxx::connection* _conn, const Stores:: SQLStore&) = 0;
    };
}