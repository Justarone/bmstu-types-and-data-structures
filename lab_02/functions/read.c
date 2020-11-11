#include "../headers/read.h"
#include "../headers/struct_utils.h"

#define READ_ERROR -2
#define TOO_MANY_STUDENTS -1
#define READED 1
#define OK 0

int read_student(student_t *const stud_list, int *const size, FILE *const f)
{
    int rc;
    int answer;
    if ((rc = fscanf(f, "%d", &answer)) != READED)
        return READ_ERROR;
    else
        stud_list[*size].home_type = answer % 2;
    if ((rc = fscanf(f, "%s", stud_list[*size].surname)) != READED)
        return READ_ERROR;
    if ((rc = fscanf(f, "%s", stud_list[*size].name)) != READED)
        return READ_ERROR;
    if ((rc = fscanf(f, "%d", &answer)) != READED)
        return READ_ERROR;
    else 
        stud_list[*size].sex = answer % 2;
    if ((rc = fscanf(f, "%f", &stud_list[*size].avg_score)) != READED)
        return READ_ERROR;
    if ((rc = fscanf(f, "%d", &stud_list[*size].adm_year)) != READED)
        return READ_ERROR;
    if (stud_list[*size].home_type == HOUSE)
    {
        if ((rc = fscanf(f, "%s", stud_list[*size].home.house.street)) != READED)
            return READ_ERROR;
        if ((rc = fscanf(f, "%u", &stud_list[*size].home.house.number)) != READED)
            return READ_ERROR;
        if ((rc = fscanf(f, "%u", &stud_list[*size].home.house.apt)) != READED)
            return READ_ERROR;
    }
    else
    {
        if ((rc = fscanf(f, "%u", &stud_list[*size].home.dorm.number)) != READED)
            return READ_ERROR;
        if ((rc = fscanf(f, "%u", &stud_list[*size].home.dorm.room)) != READED)
            return READ_ERROR;
    }
    (*size)++;
    return OK;
        
    
}

int read_from_file(student_t *const stud_list, const int max_size, FILE *const stream)
{
    int i = 0;
    while (read_student(stud_list, &i, stream) != READ_ERROR && i < max_size);
    if (i == max_size)
        return TOO_MANY_STUDENTS;
    return i - 1;
}