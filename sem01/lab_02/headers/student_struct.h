#ifndef _STUDENT_STRUCT_H_
#define _STUDENT_STRUCT_H_

#define STREET_LEN 25
#define SURNAME_LEN 40
#define NAME_LEN 20

typedef enum {HOUSE = 0, DORMITORY} home_t;
typedef enum {MAN = 0, WOMAN} sex_t;

typedef struct
{
    char street[STREET_LEN];
    unsigned int number;
    unsigned int apt;
}house_str;

typedef struct
{
    unsigned int number;
    unsigned int room;

}dormitory_str;

typedef union
{
    dormitory_str dorm;
    house_str house;
} living_place;

typedef struct
{
    home_t home_type;
    char surname[SURNAME_LEN];
    char name[NAME_LEN];
    sex_t sex;
    float avg_score;
    int adm_year;
    living_place home;
} student_t;

#endif