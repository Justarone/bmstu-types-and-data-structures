#include "queue_cdio.h"
#include <stdlib.h>

// this function receives pointer on the tail of the queue and append one more
queue_l *push_l(queue_l *const qt, const double value)
{
    queue_l *tmp = (queue_l *)malloc(sizeof(queue_l));
    // can't allocate memory for new element in queue
    if (!tmp)
        return NULL;
    tmp->next = NULL;
    tmp->data = value;
    qt->next = tmp;
    return tmp;
}