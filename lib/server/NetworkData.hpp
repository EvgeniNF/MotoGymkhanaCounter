
#ifndef SERVER_NETWORKDATA_HPP
#define SERVER_NETWORKDATA_HPP

struct NetworkData {
    /* data */
    const unsigned long int* timer_time;
    unsigned long int* time_on_esp;
    unsigned short int* status;
    std::string* host_name;
};



#endif // SERVER_NETWORKDATA_HPP