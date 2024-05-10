//
// Created by yasas on 5/1/24.
//

#pragma once
#include "../../Basics/BaseTraveller.h"

namespace Finalizer:: ResponseMapHandlers{

    class BaseResponseMapHandler{
    public:
        virtual std::string handel_response(Basics::BaseTraveller& dataTraveller) = 0;
    };

}