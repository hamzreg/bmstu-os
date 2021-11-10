#include "../inc/object.h"

int read_object(FILE *f_in, object_t *const object)
{
    int code;

    object->name = NULL;
    object->len_name = 0;

    code = my_getline(&object->name, &object->len_name, f_in);

    LOG_INFO("Line len is %d", object->len_name);

    if (!code)
    {
        LOG_ERROR("%s", "Read object error");
        return ERROR_READ_OBJECT;
    }

    code = fscanf(f_in, "%f\n", &object->mass);

    if (code != 1)
    {
        free(object->name);
        LOG_ERROR("%s", "Data type error");
        return ERROR_MASS;
    }

    if (object->mass < 0)
    {
        free(object->name);
        LOG_ERROR("%s", "Mass less 0");
        return ERROR_MASS;
    }

    code = fscanf(f_in, "%f\n", &object->volume);

    if (code != 1)
    {
        free(object->name);
        LOG_ERROR("%s", "Data type error");
        return ERROR_VOLUME;
    }

    if (object->volume < 0)
    {
        free(object->name);
        LOG_ERROR("%s", "Volume less 0");
        return ERROR_VOLUME;
    }

    return OK;
}


int copy_objects(object_t *first_object, const object_t second_object)
{
    first_object->mass = second_object.mass;
    first_object->volume = second_object.volume;
    first_object->len_name = second_object.len_name;

    first_object->name = my_strdup(second_object.name);

    if (!first_object->name)
        return ERROR_ALLOCATE;

    return COPY;
}


void print_array_object(const object_t object)
{
    printf("%s\n", object.name);
    printf("%.6f\n", object.mass);
    printf("%.6f\n", object.volume);
}


void change_objects(object_t *first_object, object_t *second_object)
{
    object_t add_object;
    add_object.name = NULL;

    if (copy_objects(&add_object, *first_object))
    {
        free(first_object->name);

        if (copy_objects(first_object, *second_object))
        {
            free(second_object->name);

            if (copy_objects(second_object, add_object))
                free(add_object.name);
        }
    }
}
