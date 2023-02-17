#include "queue_header.h"

FILE * logfile = NULL;

int main(void)
{
    if(open_logfile("queue_log.txt")) return 1;

    struct my_queue queue = {};
    if (queue_ctor(&queue)) return 1;

    double coc = 0;

    for (int i = 0; i < 240; i++)
        queue_push(&queue, i);
    
    for (int x = 0; x < 30; x++)
        queue_pop(&queue, &coc);

    queue_dump(&queue, {LOCATION}); 
    
    for (int i = 0; i < 50; i++)
        queue_push(&queue, i);
    
    for (int y = 0; y < 260; y++)
        queue_pop(&queue, &coc);

    queue_dtor(&queue);
    fclose(logfile);

    return 0;
}