#include "../inc/base_functions.h"

int check_file_size(FILE *f)
{
    long size;

    if (fseek(f, 0L, SEEK_END))
        return NO_OBJECTS;

    size = ftell(f);

    if (size <= 0)
        return NO_OBJECTS;

    return fseek(f, 0L, SEEK_SET);
}


int get_objects(const char *const file_name, array_objects_t *const objects)
{
    if (objects == NULL)
    {
        LOG_ERROR("%s", "Invalid pointer");
        return ERROR_POINTER;
    }

    FILE *f_in = fopen(file_name, "r");

    if (f_in == NULL)
    {
        LOG_ERROR("%s", "File open error");
        return ERROR_FILE_OPEN;
    }
    LOG_INFO("%s", "File open");

    int code = check_file_size(f_in);

    if (code != OK)
    {
        if (fclose(f_in) != OK)
        {
            LOG_ERROR("%s", "File close error");
            return ERROR_FILE_CLOSE;
        }
    
        return NO_OBJECTS;
    }

    code = get_array_len(f_in, objects);

    if (code != OK)
    {
        if (fclose(f_in) != OK)
        {
            LOG_ERROR("%s", "File close error");
            return ERROR_FILE_CLOSE;
        }

        return NO_OBJECTS;
    }

    rewind(f_in);

    code = create_array_objects(f_in, objects);

    if (fclose(f_in) != OK)
    {
        free_array_objects(objects);
        LOG_ERROR("%s", "File close error");
        return ERROR_FILE_CLOSE;
    }

    if (code != OK)
        return NO_OBJECTS;

    return OK;
}


void get_sorted_objects(array_objects_t *const objects)
{
    sort_array_objects(objects->array, objects->count_object - 1);
    print_array_objects(objects->array, objects->count_object);
}


void get_some_objects(const char *substring, array_objects_t *const objects)
{
    if (strcmp(substring, ALL) == 0)
        print_array_objects(objects->array, objects->count_object);
    else
        print_some_objects(substring, objects->array, objects->count_object);
}
