#pragma once

struct NetworkData {
    /* data */
    const unsigned long int* m_timerTime;
    unsigned long int* m_timeOnEsp;
    unsigned short int* m_espStatus;
    std::string* m_hostName;
};
