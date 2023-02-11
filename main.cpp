#include "queue_header.h"

FILE * logfile = NULL;

int main(void)
{
    if(open_logfile("queue_log.txt")) return 1;

    struct my_queue queue = {};
    if (queue_ctor(&queue)) return 1;

    double coc = 0;
    //scanf("%lg", &cock);

    for (int i = 0; i < 240; i++)
        queue_push(&queue, i);
    
    for (int x = 0; x < 30; x++)
        queue_pop(&queue, &coc);
    
    for (int i = 0; i < 50; i++)
        queue_push(&queue, i);

    queue_dtor(&queue);
    fclose(logfile);

    return 0;
}