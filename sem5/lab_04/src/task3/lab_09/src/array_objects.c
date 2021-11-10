#include "../inc/array_objects.h"

int get_array_len(FILE *f_in, array_objects_t *const objects)
{
    object_t add;
    objects->count_object = 0;

    while (!feof(f_in))
    {
        if (!read_object(f_in, &add))
        {
            objects->count_object++;
            free(add.name);
            LOG_INFO("%s", "Memory frees");
        }
        else
            return ERROR_READ_OBJECT;
    }

    if (objects->count_object <= 0)
        return NO_OBJECTS;

    return OK;
}


int create_array_objects(FILE *f_in, array_objects_t *const objects)
{
    objects->array = malloc(objects->count_object * sizeof(objects->array[0]));

    if (!objects->array)
        return ERROR_ALLOCATE;

    for (int i = 0; i < objects->count_object; i++)
        read_object(f_in, &objects->array[i]);

    return OK;
}


void sort_array_objects(object_t *objects, const int end)
{
    if (!objects)
        return;

    if (end < 0)
        return;

    int left = 0, right = end, middle = (left + end) / 2;
    float now_density, average_density = objects[middle].mass /
        objects[middle].volume;
 
    while (left <= right)
    {
        now_density = objects[left].mass / objects[left].volume;

        while (now_density < average_density)
        {
            left++;
            now_density = objects[left].mass / objects[left].volume;
        }

        now_density = objects[right].mass / objects[right].volume;

        while (now_density > average_density)
        {
            right--;
            now_density = objects[right].mass / objects[right].volume;
        }

        if (left <= right)
        {
            if (left != right)
                change_objects(&objects[left], &objects[right]);

            left++;
            right--;
        }
    }

    if (right > 0)
        sort_array_objects(objects, right);

    if (left < end)
        sort_array_objects(objects + left, end - left);
}


void print_array_objects(const object_t *const array_objects, const int count)
{
    for (int i = 0; i < count; i++)
        print_array_object(array_objects[i]);
}


void print_some_objects(const char *const substring, const object_t *const objects, const int count)
{
    size_t len = strlen(substring);

    for (int i = 0; i < count; i++)
        if (strncmp(objects[i].name, substring, len) == 0)
            print_array_object(objects[i]);
}

void free_array_objects(array_objects_t *objects)
{
    for (int i = 0; i < objects->count_object; i++)
        free(objects->array[i].name);

    free(objects->array);
}
