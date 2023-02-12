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

    return 0;
}


int queue_push(struct my_queue * queue, elem push_value)
{
    assert(queue);
    assert(isfinite(push_value));

    if (queue->head == ((queue->tail + 1) & (BUFF_SIZE - 1)))
    {
        fprintf(logfile, "ERROR: queue overflow. Element %lg was not added to queue.\n\n\n", push_value);
        return 1;
    }

    queue->data[queue->tail] = push_value;
    queue->tail = (queue->tail + 1) & (BUFF_SIZE - 1);

    fprintf(logfile, "%lg was pushed to queue \"%s\"\ntail - %d\nhead - %d\n\n\n", push_value, queue->queue_info.name, queue->tail, queue->head);

    return 0;
}


int queue_pop(struct my_queue * queue, elem * out_value)
{
    assert(queue);

    if (queue->tail == queue->head)
    {
        fprintf(logfile, "ERROR: queue is empty. Cannot pop element.\n\n\n");
        return 1;
    }

    *out_value = queue->data[queue->head];
    queue->data[queue->head] = POISON;
    queue->head = (queue->head + 1) & (BUFF_SIZE - 1);

    fprintf(logfile, "%lg was poped from queue \"%s\"\ntail - %d\nhead - %d\n\n\n", *out_value, queue->queue_info.name, queue->tail, queue->head);

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


int queue_dump(struct my_queue * queue, const char * func, const char * file, size_t line)
{
    assert(queue);

    fprintf(logfile, "    DUMP from:\nfile - %s\nfunc - %s\nline - %lu\nqueue - \"%s\"\n", file, func, line, queue->queue_info.name);
    
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

    return 0;
}