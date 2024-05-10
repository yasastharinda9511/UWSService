//
// Created by yasas on 4/28/24.
//

#pragma once

namespace Connectors{

    class BaseConnector{

    public:
        virtual ~BaseConnector() = default;
        virtual bool connect() = 0;
        virtual bool disconnect() = 0;
        virtual void init() = 0;
    };

}
