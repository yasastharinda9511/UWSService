//
// Created by yasas on 4/29/24.
//

#pragma once

#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <future>

namespace EventBus{

    template<typename DataType>
    class EventBus{

        using EventHandler = std::function<void(DataType&)>;
    private:
        std::unordered_map<std::string, std::vector<EventHandler>> _event_handlers_map;
        static inline EventBus<DataType>* instance = nullptr;
    public:
        EventBus() = default;
        EventBus(const EventBus&) = delete;
        EventBus& operator=(const EventBus&) = delete;

        static EventBus<DataType>* get_event_bus()
        {
            if (instance == nullptr){
                instance = new EventBus<DataType>();
            }
            return instance;
        }
        void subscribe(const std::string& event_bus_name, const EventHandler& event_handler){
            _event_handlers_map[event_bus_name].push_back(event_handler);
        }
        void publish(const std::string& event_bus_name, DataType& data_type){
            auto ite = _event_handlers_map.find(event_bus_name) ;
            if(ite == _event_handlers_map.end()) return ;

            for(const auto& event_handler: _event_handlers_map[event_bus_name]){
                event_handler(data_type);
            }
        }
        void publish_async(const std::string& event_bus_name, DataType& data_type){
            std::async(std::launch::async,[this, &event_bus_name, &data_type](){
                this->publish(event_bus_name, data_type);
            });
        }

    };

}
