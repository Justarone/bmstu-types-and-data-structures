#include "trees.h"
#include "hash.h"
#include "file.h"
#include "io_funcs.h"
#include <stdlib.h>
#include <stdio.h>
#include "additional_structs.h"
#include "timer.h"

#define OK 0

#define TREE 0
#define ATREE 1
#define HASH_TABLE 2
#define FILE_LIST 3

#define FILENAME "data.txt"
#define READED 1
#define BUFF_LEN 255

extern char buffer[BUFF_LEN];

int main()
{
    FILE *f;
    int choice;
    int new_base = 0;
    int err_code = 1;
    stat_t stat_array[4] = {};
    uint64_t start = 0;

    hash_t *table = NULL;
    node_t *vertex_b = NULL;
    node_t *vertex = NULL;

    // count size of file
    int file_size = 0;
    char c;
    f = fopen(FILENAME, "r");
    while ((c = fgetc(f)) != EOF && c != '\0')
    {
        if (c == '\n')
            file_size++;
    }

    table = init_table(next_simple(file_size));

    print_menu();
    while (scanf("%d", &choice) != READED)
    {
        puts("That's not a number, try again:");
        char c;
        while (scanf("%c", &c) == READED && c != '\n')
            ;
    }
    while (1)
    {
        printf("\n\n");
        switch (choice)
        {
        case 0:
            puts("EXIT...");
            goto free_label;

        case 1:
            f = fopen(FILENAME, "r");
            if (!f)
            {
                printf("Error, while opening file. Sorry.\n");
                break;
            }
            input_hash_table(&table, f);
            fseek(f, 0, SEEK_SET);
            input_tree(&vertex_b, f);
            tree_to_atree(vertex_b, &vertex);
            printf("Input from file is done.\n");
            fclose(f);
            break;
        case 2:
            printf("Enter word to delete: ");
            if (scanf("%254s", buffer) != READED)
            {
                printf("Can't read the word, sorry.\n");
                break;
            }
            start = tick();
            stat_array[ATREE].comp_num = 0;
            vertex = remove_a(vertex, buffer, &stat_array[ATREE]);
            stat_array[ATREE].time = tick() - start;

            stat_array[TREE].comp_num = 0;
            start = tick();
            vertex_b = remove_b(vertex_b, buffer, &stat_array[TREE]);
            stat_array[TREE].time = tick() - start;

            stat_array[HASH_TABLE].comp_num = 0;
            start = tick();
            err_code = remove_h(table, buffer, &stat_array[HASH_TABLE]);
            stat_array[HASH_TABLE].time = tick() - start;

            stat_array[FILE_LIST].comp_num = 0;
            f = fopen(FILENAME, "r+");
            start = tick();
            remove_f(f, buffer, &stat_array[FILE_LIST]);
            stat_array[FILE_LIST].time = tick() - start;
            fclose(f);

            if (err_code)
                printf("No such word in structures\n");
            else
                printf("Deletion is done.\n");
            break;
        case 3:
            print_tree(vertex_b, 0);
            break;
        case 4:
            print_tree(vertex, 0);
            break;
        case 5:
            print_hash_table(table);
            break;
        case 6:
            if (err_code)
                printf("Last deletion wasn't correct..."
                       " (or there was no deletion still)\n");
            else
                print_stat(stat_array);
            break;
        case 7:
            printf("Input new base of table: ");
            if (scanf("%d", &new_base) != READED)
            {
                printf("Can't read base value\n");
                break;
            }
            if (new_base <= 0)
            {
                printf("Wrong value of base\n");
                break;
            }

            int rc = rebase(&table, new_base);
            if (rc)
                printf("Can't create table with new base (too much collisions)\n");
            break;
        default:
            printf("That's not right number, please, try again.\n");
            break;
        }
        print_menu();
        while (scanf("%d", &choice) != READED)
        {
            puts("\n\nThat's not a number, try again: ");
            char c;
            while (scanf("%c", &c) == READED && c != '\n')
                ;
        }
    }
free_label:
    destruct_table(table);
    clean_atree(vertex);
    clean_tree(vertex_b);
    return OK;
}