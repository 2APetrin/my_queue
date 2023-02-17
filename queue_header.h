#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__
#define queue_ctor(queue) _queue_ctor((queue), var_info {#queue, LOCATION})


//! @brief size of cyclic buffer. Only powers of 2

const int BUFF_SIZE = 128; 


//! @brief type of elements inside queue

typedef double elem;       


//! @brief value on place of deleted/free elements

const elem POISON = NAN;


//! @brief extern logfile variable

extern FILE * logfile;     


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
    size_t       line;
};


//! @struct location
//! @brief locaion of call
//!
//! @var func - function, where calling function is
//! @var file - file, where calling function is
//! @var line - line, where calling function is

struct location_info
{
    const char * func;
    const char * file;
    size_t       line;
};


//! @brief codes of errors for queue

enum queue_err_codes
{
    BAD_TAIL_VAL = 1,
    BAD_DATA_PTR = 2,
    BAD_VAR_NAME = 4,
    BAD_VAR_FUNC = 8,
    BAD_VAR_FILE = 16,
    BAD_LINE_VAL = 32,
    BAD_HEAD_VAL = 64
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
    unsigned int head;
    unsigned int tail;
    double     * data;

    struct var_info queue_info;
};


//! @brief creates data arrange and initializes head and tail
//!
//! @param [out] queue ptr to our queue object
//! @param [in]  info  information about our queue object creation
//!
//! @return 1 if error, 0 if ok

int _queue_ctor(struct my_queue * queue, struct var_info info);


//! @brief adds number to our queue
//!
//! @param [out] queue ptr to our queue object
//! @param [in]  push_value push value
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
//! @param [in] queue   ptr to our queue object
//! @param [in] loc_inf struct with calling function location
//!
//! @return 1 if error, 0 if ok

int queue_dump(struct my_queue * queue, struct location_info loc_inf);


//! @brief verifys our queue 
//! 
//! @param [in] queue ptr to our queue object
//! @param [in] loc_inf struct with calling function location
//!
//! @return if error is catched, prints in logfile diagnostics and turns off our queue by abort()

int queue_verify(struct my_queue * queue, struct location_info loc_inf);


//! @brief checks for errors
//! 
//! @param [in]  queue ptr to our queue object
//! @param [out] err_number error code. 0 if all is ok
//!
//! @return error code

int queue_err_check(struct my_queue * queue, unsigned char * err_count);