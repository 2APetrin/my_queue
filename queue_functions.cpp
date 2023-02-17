#include "queue_header.h"


int _queue_ctor(struct my_queue * queue, struct var_info info)
{
    assert(queue);
    assert(info.file);
    assert(info.func);
    assert(info.name);

    queue->data = (elem *) calloc (BUFF_SIZE, sizeof(elem));

    for (int i = 0; i < BUFF_SIZE; i++)
    {
        queue->data[i] = POISON;
    }

    if (!queue->data) // если !указатель = 1 т.е. указатель = 0
    {
        fprintf(logfile, "ERROR: cannot calloc in queue_ctor\n\n\n");
        return 1;
    }

    queue->head = 0;
    queue->tail = 0;

    queue->queue_info = info;

    fprintf(logfile, "Queue \"%s\" was constructed\n\n\n", queue->queue_info.name);

    queue_verify(queue, {info.func, info.file, info.line});

    return 0;
}


int queue_push(struct my_queue * queue, elem push_value)
{
    assert(queue);
    assert(isfinite(push_value));

    queue_verify(queue, {LOCATION});

    if (queue->head == ((queue->tail + 1) & (BUFF_SIZE - 1)))
    {
        fprintf(logfile, "WARNING: queue overflow. Element %lg was not added to queue.\n\n\n", push_value);
        return 1;
    }

    queue->data[queue->tail] = push_value;
    queue->tail = (queue->tail + 1) & (BUFF_SIZE - 1);

    fprintf(logfile, "%lg was pushed to queue \"%s\"\ntail - %u\nhead - %u\n\n\n", push_value, queue->queue_info.name, queue->tail, queue->head);

    queue_verify(queue, {LOCATION});

    return 0;
}


int queue_pop(struct my_queue * queue, elem * out_value)
{
    assert(queue);

    queue_verify(queue, {LOCATION});

    if (queue->tail == queue->head)
    {
        fprintf(logfile, "WARNING: queue is empty. Cannot pop element.\n\n\n");
        return 1;
    }

    *out_value = queue->data[queue->head];
    queue->data[queue->head] = POISON;
    queue->head = (queue->head + 1) & (BUFF_SIZE - 1);

    fprintf(logfile, "%lg was poped from queue \"%s\"\ntail - %u\nhead - %u\n\n\n", *out_value, queue->queue_info.name, queue->tail, queue->head);

    queue_verify(queue, {LOCATION});

    return 0;
}


int open_logfile(const char * filename)
{
    if ((logfile = fopen(filename, "w")) == NULL)
    {
        printf("Cannot open logfile. Programm shutdown\n");
        return 1;
    }

    return 0;
}


int queue_dtor(struct my_queue * queue)
{
    assert(queue);
    assert(queue->data);

    queue_verify(queue, {LOCATION});

    for (int i = 0; i < BUFF_SIZE; i++)
    {
        queue->data[i] = POISON;
    }

    free(queue->data);

    queue->head = 0;
    queue->tail = 0;

    queue->queue_info.file = NULL;
    queue->queue_info.func = NULL;
    queue->queue_info.line = 0;

    fprintf(logfile, "Queue \"%s\" was destructed\n\n\n", queue->queue_info.name);
    queue->queue_info.name = NULL;

    return 0;
}


int queue_dump(struct my_queue * queue, struct location_info loc_inf)
{
    assert(queue);

    queue_verify(queue, {LOCATION});;

    fprintf(logfile, "    DUMP from:\nfile - %s\nfunc - %s\nline - %lu\nqueue - \"%s\"\n", loc_inf.file, loc_inf.func, loc_inf.line, queue->queue_info.name);
    fprintf(logfile, "\nNot free elements:\n[index] - [value]\n");
    
    for (int i = 0; i < BUFF_SIZE; i++)
    {
        if (!isnan(queue->data[i]))
        {
            if (!fprintf(logfile, "[%3d] - [%lg]\n", i, queue->data[i]))
            {
                return 1;
            }  
        }
    }

    fprintf(logfile, "\n\n");

    queue_verify(queue, {LOCATION});

    return 0;
}


int queue_verify(struct my_queue * queue, struct location_info loc_inf)
{
    assert(queue);

    unsigned char error_count = 0;

    int cnt = 0;
    int error_number = queue_err_check(queue, &error_count);
    int switch_param = 0;

    //printf("%d\n", error_number);
    //printf("%d\n", error_count);

    if ((error_number))
    {
        fprintf(logfile, "-------------------------------------------------------------------------------------\n");
        fprintf(logfile, "   ERROR!!!\nError catched in:\nfunc - %s\nfile - %s\nline - %lu\n\n\n", loc_inf.func, loc_inf.file, loc_inf.line);

        for (size_t i = 0; cnt < error_count && i < sizeof(int); i++)
        {   
            switch_param = error_number & (1 << i);
            switch(switch_param)
            {
                case BAD_DATA_PTR:
                    fprintf(logfile, "Data have NULL pointer\n");
                    cnt++;
                    break;
                
                case BAD_HEAD_VAL:
                    fprintf(logfile, "Bad head value\n");
                    cnt++;
                    break;
                
                case BAD_LINE_VAL:
                    fprintf(logfile, "Bad line count (<= 0)\n");
                    cnt++;
                    break;
                
                case BAD_TAIL_VAL:
                    fprintf(logfile, "Bad tail value\n");
                    cnt++;
                    break;
                
                case BAD_VAR_FILE:
                    fprintf(logfile, "File name have NULL pointer\n");
                    cnt++;
                    break;
                
                case BAD_VAR_FUNC:
                    fprintf(logfile, "Function name have NULL pointer\n");
                    cnt++;
                    break;
                
                case BAD_VAR_NAME:
                    fprintf(logfile, "Variable name have NULL pointer\n");
                    cnt++;
                    break;
                
                default: 
                    break;
            }
        }

        fprintf(logfile, "-------------------------------------------------------------------------------------\n\n");

        fclose(logfile);
        abort();
    }

    return 0;
}


int queue_err_check(struct my_queue * queue, unsigned char * err_count)
{
    assert(queue);
    assert(err_count);

    int err_code = 0;

    if (queue->tail > BUFF_SIZE)
    {
        err_code   += BAD_TAIL_VAL;
        *err_count += 1;
    }
    
    if (queue->head > BUFF_SIZE)
    {
        err_code   += BAD_HEAD_VAL;
        *err_count += 1;
    }
    
    if (!queue->data)
    {
        err_code   += BAD_DATA_PTR;
        *err_count += 1;
    }
    
    if (!queue->queue_info.func)
    {
        err_code   += BAD_VAR_FUNC;
        *err_count += 1;
    }
    
    if (!queue->queue_info.file)
    {
        err_code   += BAD_VAR_FILE;
        *err_count += 1;
    }
    
    if (queue->queue_info.line <= 0)
    {
        err_code  += BAD_LINE_VAL;
        *err_count += 1;
    }

    return err_code;
}