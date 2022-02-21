#pragma once

#include <string>

struct NetworkData {
    /* data */
    const unsigned long int* timerTime;
    unsigned long int* timeOnEsp;
    unsigned short int* espStatus;
    std::string* hostName;
};
