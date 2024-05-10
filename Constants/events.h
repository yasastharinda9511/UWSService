//
// Created by yasas on 4/30/24.
//

#pragma once

namespace Constants {
    enum class Events{

        ADD_STUDENT,
        UPDATE_STUDENT,
        GET_ALL_STUDENTS,
        ERROR   // not an event, generate while processing the above events
    };
}