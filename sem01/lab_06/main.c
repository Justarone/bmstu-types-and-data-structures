#include "trees.h"
#include "hash.h"
#include "io_funcs.h"
#include <stdlib.h>
#include <stdio.h>

#define OK 0

#define FILENAME "data.txt"
#define READED 1
#define BASE 10
#define BUFF_LEN 255

extern char buffer[BUFF_LEN];

int main()
{
    FILE *f;
    int choice;
    hash_t *table = NULL;
    node_t *vertex_b = NULL;
    node_t *vertex = NULL;

    table = init_table(BASE);

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
            input_hash_table(table, f);
            fseek(f, 0, SEEK_SET);
            input_tree(&vertex_b, f);
            tree_to_atree(vertex_b, &vertex);
            printf("Input from file is done.\n");
            fclose(f);
            break;
        case 2:
            printf("Enter word to delete:");
            if (scanf("%254s", buffer) != READED)
            {
                printf("Can't read the word, sorry.\n");
                break;
            }
            remove_h(table, buffer);
            vertex = remove_a(vertex, buffer);
            vertex_b = remove_b(vertex_b, buffer);
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
            printf("in develop mode;\n");
            break;
        default:
            printf("That's not right number, please, try again.\n");
            break;
        }
        print_menu();
        while (scanf("%d", &choice) != READED)
        {
            puts("\n\nThat's not a number, try again:");
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