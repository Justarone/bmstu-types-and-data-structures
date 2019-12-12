#include "io_funcs.h"

#define OK 0

#define READED 1
#define BUFF_LEN 255

char buffer[BUFF_LEN] = {0};

void print_menu()
{
    printf("MENU\n"
           "0. Exit;\n"
           "1. Read data from file;\n"
           "2. Delete word;\n"
           "3. Show binary tree;\n"
           "4. Show AUS-tree;\n"
           "5. Show hash-table;\n"
           "6. Show comparison\n");
}

// ввод хэш таблицы
int input_hash_table(hash_t *const table, FILE *const stream)
{
    while (fscanf(stream, "%s", buffer) == READED)
        add(table, buffer);
    return OK;
}

// ввод ддп дерева
int input_tree(node_t **vertex_b, FILE *const stream)
{
    while (fscanf(stream, "%s", buffer) == READED)
        *vertex_b = insert_b(*vertex_b, buffer);
    return OK;
}

// перенос ддп в авл-дерево
void tree_to_atree(node_t *vertex_b, node_t **vertex)
{
    if (!vertex_b)
        return;
    *vertex = insert(*vertex, vertex_b->value);
    tree_to_atree(vertex_b->left, vertex);
    tree_to_atree(vertex_b->right, vertex);
}

void print_hash_table(const hash_t *const table)
{
    printf("\033[1;29m|  HASH  |        VALUE        |\n");
    node_h *elem;
    for (int i = 0; i < table->base; i++)
    {
        elem = table->data[i];
        // if no elems with this hash go to next one
        if (!elem)
            continue;
        // first print of element (first element)
        printf("|  %4d  |   %15s   |\n", i, elem->data);
        elem = elem->next;
        // print more elems with same hash (collision, marked with red colour)
        while (elem)
        {
            printf("|  %4d  |   \033[1;31m%15s \033[1;37m  |\n", i, elem->data);
            elem = elem->next;
        }
    }
}

void print_vertex(const node_t *const vertex, const int depth)
{
    for (int i = 0; i < depth; i++)
        printf("%10s", " ");
    printf("%10s\n", vertex->value);
}

void print_tree(const node_t *const vertex, const int depth)
{
    if (!vertex)
        return;
    printf("\033[1;29m");
    print_tree(vertex->right, depth + 1);
    print_vertex(vertex, depth);
    print_tree(vertex->left, depth + 1);
    printf("\033[1;29m");
}
