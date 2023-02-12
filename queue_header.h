#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define queue_ctor(queue) _queue_ctor((queue), var_info {#queue, LOCATION})


const int BUFF_SIZE = 128; //size of cyclic buffer

typedef double elem;       // type of elements inside queue

const elem POISON = NAN;   // value on place of deleted/free elements

extern FILE * logfile;     // extern logfile variable


//! @struct variable information
//! @brief  struct with information about variable
//! 
//! @var name - struct queue variable name
//! @var func - function where queue variable was created
//! @var file - file where queue variable was created
//! @var line - number of line where variable was created

struct var_info
{
    const char * name;
    const char * func;
    const char * file;
    int          line;
};


//! @struct my own queue
//! @brief  main struct of our queue
//! 
//! @var head - index of first free space next to first item in queue
//! @var tail - index of last item in queue
//! @var data - arrange/cyclic buffer for queue
//! @var queue_info - struct with all the information about queue varible

struct my_queue
{
    unsigned char head;
    unsigned char tail;
    double      * data;

    struct var_info queue_info;
};


//! @brief creates data arrange and initializes head and tail
//!
//! @param [out] queue ptr to our queue object
//! @param [in] info  information about our queue object creation
//!
//! @return 1 if error, 0 if ok

int _queue_ctor(struct my_queue * queue, struct var_info info);


//! @brief adds number to our queue
//!
//! @param [out] queue ptr to our queue object
//! @param [in] push_value push value
//!
//! @return 1 if error, 0 if ok

int queue_push(struct my_queue * queue, elem push_value);


//! @brief pops number from our queue
//!
//! @param [in]  queue ptr to our queue object
//! @param [out] out_value ptr to variable where value pops
//!
//! @return 1 if error, 0 if ok

int queue_pop(struct my_queue * queue, elem * out_value);


//! @brief opens txt file for logs
//!
//! @param [in] filename name of txt file to open
//!
//! @return 1 if error, 0 if ok

int open_logfile(const char * filename);


//! @brief destructor of our queue
//!
//! @param [out] queue ptr to our queue object
//!
//! @return 1 if error, 0 if ok

int queue_dtor(struct my_queue * queue);


//! @brief prints all non POISON elements in queue
//!
//! @param [in] queue ptr to our queue object
//! @param [in] func name of funtion, where function was called
//! @param [in] file name of file, where function was called
//! @param [in] line index of line, where dump was called
//!
//! @return 1 if error, 0 if ok

int queue_dump(struct my_queue * queue, const char * func, const char * file, size_t line);
