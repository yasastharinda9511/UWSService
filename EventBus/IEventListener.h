//
// Created by yasas on 5/1/24.
//

#pragma once

#include <functional>


namespace EventBus{
    template<typename DataType>
    class IEventListener {
        virtual void listen(DataType& data)=0;
    };
}