cmake_minimum_required(VERSION 3.27)
project(UWSService)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        /home/yasas/CLionProjects/3rd_party_libs/include
        /home/yasas/CLionProjects/3rd_party_libs/include/uWebSockets
)

enable_testing()  # Enables integration with CTest
add_subdirectory(test)

link_directories(/home/yasas/CLionProjects/3rd_party_libs/libs
        /home/yasas/CLionProjects/3rd_party_libs/lib64
        /usr/local/lib64
        /usr/locl/lib
        /usr/lib/
        /usr/lib64/)

add_executable(UWSService main.cpp
        Services/Service.cpp
        Services/Service.h
        Services/Constants/HttpMethods.h
        Models/Student.h
        Controller/BaseController.h
        Controller/StudentController.h
        Basics/DataTraveller.h
        Connector/BaseConnector.h
        EventBus/EventBus.h
        Connector/DBConnector/DBConnector.cpp
        Connector/DBConnector/DBConnector.h
        Connector/DBConnector/DbScriptHandlers/BaseHandler.h
        Connector/DBConnector/DbScriptHandlers/AddStudentHandler.h
        Basics/BaseTraveller.h
        Constants/sql_query_types.h
        Stores/SqlStore.h
        Constants/events.h
        EventBus/IEventListener.h
        DataFinalizer/DataFinalizer.cpp
        DataFinalizer/DataFinalizer.h
        DataFinalizer/DataFinalizeHandlers/BaseResponseMapperHandler.h
        DataFinalizer/DataFinalizeHandlers/AddStudentResponseHandler.h
        Constants/errorCodes.h
        DataFinalizer/DataFinalizeHandlers/ErrorHandler.h
        Models/error.h
        Connector/DBConnector/DbScriptHandlers/GetAllStudentsHandler.h
        DataFinalizer/DataFinalizeHandlers/GetAllStudentsResponseHandler.h
        Constants/event_bus.h
        Basics/AsynExecutor/ExtendedPromise.h
        Basics/AsynExecutor/AsyncChain.h
        Basics/AsynExecutor/AsyncExceptions/AsyncException.h
        Basics/Factory/Factories.h
        Basics/AsynExecutor/AsyncExecutor.h
        Basics/AsynExecutor/Task.h
        Basics/AsynExecutor/LockFreeQueue/LockFreeQueue.h
        Basics/AsynExecutor/TaskExecutors.h
        Basics/AsynExecutor/TaskBase.h
        Basics/AsynExecutor/AsyncExceptions/LockFreeQueueEmptyException.h
)

target_link_libraries(UWSService uSockets
        z
        ssl
        crypto
        pqxx
        fmt)
