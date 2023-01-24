#ifndef _RASPPI_COMMS_H_
#define _RASPPI_COMMS_H_
#include "api.h"

class RasppiComms {
    public:

    RasppiComms();

    ~RasppiComms();

    bool listen();

    void read_256_from_buff(char * array_out);

    void stopListen();

    private:

    // static pthread_mutex_t __mtx;
    static pros::Mutex __mtx;
    pthread_t __listen1;
    bool __listen_active;
    char * __buffer;
    int __buffer_end; // index last written to in the buffer
    const char __tag [4] = "IVR";
    const char __ack [4] = "ACK";
    const int __tag_size = 3;
    const int __ack_size = 3;

    int __ready;

    static void __listen(void *context);

    void __establishConnection();

    int __verify_buffer();

    int __verify_ack();

    int __compare_tag(char *cmp);

    int __scpy(char * src);

    // void __stopListen();

};

#endif // __RASPPI_COMMS_H_