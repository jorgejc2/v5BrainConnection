#include "../include/rasppi_comms.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
// #include "../include/api.h"
// #include "../include/pros/apix.h"

using namespace std;

// pthread_mutex_t RasppiComms::__mtx = PTHREAD_MUTEX_INITIALIZER;
pros::Mutex RasppiComms::__mtx; // mutex lock for altering __buffer and __ready 

/*
Description: Creates an instance of RasspiComms object
Inputs: None
Ouputs: None
Returns: None
Effects: Initializes private buffer and thread for listening from serial port
*/
RasppiComms::RasppiComms()  {
    __listen_active = false;
    __buffer = new char[256];
    __buffer_end = 0;
    cout << "Instantiated object" << endl;
    __ready = 0;
    __lastOrder = 0;
    listen();  // start thread which listens from the serial bus
    // __establishConnection();  // Note that this is a blocking function so RaspPi MUST send acknowledge message
};

/*
Description: Destroys instance
Inputs: None
Outputs: None
Returns: None
Effects: Ends thread responsible for listening from serial port
*/
RasppiComms::~RasppiComms() {
    stopListen();
    // delete[] __tag;
    // delete[] __buffer;
};

/*
Description: Creates a thread (in prosv5 called a task) for listening to serial port
Inputs: None
Outputs: None
Returns: None
Effects: Creates a thread
*/
bool RasppiComms::listen() {
    __listen_active = true;
    // return (pthread_create(&__listen1, NULL, __listen, this) == 0);
    pros::Task my_task(__listen, (void *)this, "ListenTask");
    return 1;
};

/*
Description: Read from private buffer
Inputs: None
Outputs:
    char* array_out -- array to write into from private buffer
Returns: None
Effects: Takes mtx lock for a period of time
*/
void RasppiComms::read_256_from_buff(char * array_out) {
    // pthread_mutex_lock(&__mtx);
    // cout<<"Begin str copy"<<endl;
    bool take;
    while (1) {
        take = __mtx.take();
        // if (take)
        //     // cout<<"take successful"<<endl;
        // else {
        //     // cout<<"fail take"<<endl;
        //     // __mtx.give();
        //     continue;
        // }
        if (!take)
            continue;
            
        if (__buffer_end > 0)
            strncpy(array_out, __buffer, __buffer_end + 1);
        else {
            /* my personal method of determining when a buffer copy has failed */
            array_out[0] = 'P';
            array_out[1] = '\n';
        }
        fill(__buffer, __buffer + 256, '\n');
        __buffer_end = 0;
        __ready = 1;
        // pthread_mutex_unlock(&__mtx);
        __mtx.give();
        // cout<<"end str copy"<<endl;
        break;
    }

    return;
}

/*
Description: Stops the thread
Inputs: None
Outputs: None
Returns: None
Effects: lowers private __listen_active flag
*/
void RasppiComms::stopListen() {
    __listen_active = false;
    // __stopListen();
};


void RasppiComms::__establishConnection() {
    int ack = 0;
    int match = 0;
    char message [7] = "IVRACK";
    while (!ack) {
        // fwrite(message, sizeof(char), 9, stdout);
        fwrite(message, sizeof(char), 7, stdout);
        __mtx.take();
        // fwrite(__buffer, sizeof(char), 256, stdout);
        if (__verify_buffer() != -1 && __verify_ack() != -1) 
            ack = 1;
        __mtx.give();
    }
}

/* 
Note that since this reads the buffer, it should only be invoked when the lock is held.
Returns the position in the buffer to start reading from which is just the tag size
*/
int RasppiComms::__verify_buffer() {
    int match = 0;
    for (int i = 0; i <= __tag_size; i++) {
        if (i == __tag_size)
            match = 1;
        else if (__buffer[i] != __tag[i])
            break;
    }
    if (match)
        return __tag_size;
    else
        return -1;
}

int RasppiComms::__verify_ack() {
    int match = 0;
    for (int i = 0; i <= __ack_size; i++) {
        if (i == __ack_size)
            match = 1;
        else if (__buffer[__tag_size + i] != __ack[i])
            break;
    }
    if (match)
        return __ack_size;
    else    
        return -1;
}

/*
Description: Thread responsible for writing to private buffer from serial port
Inputs:
    void* context -- reference to RasppiComms object
Outputs: None
Returns: None
Effects: Repeatedly takes mtx lock
*/
void RasppiComms::__listen(void *context) {
    char temp[256]; // temp that constantly reads from micro usb
    char mes_ack[9] = "IVR0ACK\n"; // acknowledge signal to send to rasppi if message was received (otherwise it will keep spamming the message)
    RasppiComms* obj_inst = (RasppiComms*)context; // cast context to object reference

    /* main work that does not end until __listen_active is lowered */
    while (obj_inst->__listen_active) {

        /* serial port read*/
        fread(temp, sizeof(char), 256, stdin);

        /* if no matching tag or buffer is full, continue */
        if ((!obj_inst->__compare_tag(temp)) || (obj_inst->__buffer_end >= 255))
            continue;

        /* check for correct ordered message */
        if(temp[3] != obj_inst->__lastOrder) {
            fwrite(mes_ack, sizeof(char), 9, stdout); // rasppi probably didnt get ack
            continue;
        }

        /* this point is only reached if the tag was matched and the buffer has space to copy values */

        /* take lock */
        obj_inst->__mtx.take();

        /* check if the object is ready to received the message (__ready is set after read_256_from_buffer is called)*/
        if (obj_inst->__ready == 0) {
            obj_inst->__mtx.give();
            continue;
        }

        /* clear and write to buffer */
        fill(obj_inst->__buffer, obj_inst->__buffer + 256, '\n');
        obj_inst->__buffer_end = 0;
        // obj_inst->__scpy(temp + obj_inst->__tag_size);
        obj_inst->__scpy(temp);
        obj_inst->__ready = 0;

        /* release lock */
        obj_inst->__mtx.give();

        /* write to rasppi that message was received */
        fwrite(mes_ack, sizeof(char), 9, stdout);

        if (obj_inst->__lastOrder < 9) {
            obj_inst->__lastOrder++;
            mes_ack[3] = obj_inst->__lastOrder;
        }
        else {
            obj_inst->__lastOrder = 0;
            mes_ack[3] = 0;
        }

        
    }

    /* free private buffer and resolve thread */
    cout<<"destroying object instance"<<endl;
    delete[] obj_inst->__buffer;
    return;
};

// void RasppiComms::__stopListen() {
//     (void) pthread_join(__listen1, NULL);
// }

/*
Description: compares tag from message to private tag
Inputs: 
    char* cmp -- message whose tag is to be checked
Outputs: None
Returns: 
    int same -- 1 if tag is correct (thus a valid message) else 0
Effects: None
*/
int RasppiComms::__compare_tag(char *cmp) {
    int same = 0;
    for (int j = 0; j < 256 - __tag_size; j++) {
        for (int i = 0; i < __tag_size; i++) {
            same = 1;
            if (cmp[i] != __tag[i]) {
                same = 0;
                break;
            }
        }
    }
    return same;
}

/*
Description: copies private buffer into an array
Inputs: None
Outputs: 
    char* src -- pointer to array that will contain the buffer's output
Returns:
    int idx -- number of characters copied into the output array
Effects: None
*/
int RasppiComms::__scpy(char *src) {
    int idx = 0;
    while ((src[idx] != '\n') && (__buffer_end < 256)) {
        __buffer[idx] = src[idx];
        ++__buffer_end;
        ++idx;
    }

    return idx; // idx reflects number of characters copied
}

