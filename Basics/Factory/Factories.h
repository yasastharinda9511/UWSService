//
// Created by yasas on 5/12/24.
//


#include <utility>
#include <cstddef>
#include <queue>

namespace Basics::Factories{

    template<typename T>
    class Factory {

    public:

        template<typename... Args>
        explicit Factory(size_t initial_size = 10 , Args&&... args){
            for(size_t i = 0 ; i < initial_size ; i ++ ){
                _object_pool_queue.emplace(new T(std::forward<Args>(args)...));
            }
        }
        template<typename... Args>
        T& create(Args&&... args){

            if(!_object_pool_queue.empty()){
                T* pool_object = _object_pool_queue.front();
                _object_pool_queue.pop();
                return *pool_object;
            }
            return *(new T(std::forward<Args>(args)...));
        }

        void destroy(T* factory_object){
            _object_pool_queue.emplace(factory_object);
        }

    private:
        std::queue<T*> _object_pool_queue;

    };

}
