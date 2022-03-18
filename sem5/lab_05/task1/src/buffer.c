#include "buffer.h"

int init_buffer(buffer_t *const buffer)
{
    if (!buffer)
    {
        return -1;
    }

    memset(buffer, 0, sizeof(buffer_t));

    buffer->element = 'a';

    return 0;
}

int read_buffer(buffer_t *const buffer, char *const elem)
{
    if (!buffer)
    {
        return -1;
    }

    *elem = buffer->data[buffer->rpos++];

    return 0;
}

int write_buffer(buffer_t *const buffer, const char elem)
{
    if (!buffer)
    {
        return -1;
    }

    buffer->data[buffer->wpos++] = elem;

    return 0;
}