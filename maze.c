/*
Autor: Jakub Smička
Login: xsmickj00
Projekt 2 - bludiště
** gcc -std=c11 -Wall -Wextra -Werror maze.c -o maze
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*
TODO:
1) Hledat cestu podle L/P. Napadá mě alg podle toho, že když jsem přišel z jedné hrany, tak další bude o jedno větší např 1 -> 2,..
2) Implementace argv programu
3) Validace matice
4) bonus řešení short test
*/
// Implementace 1. Podúkolu - struktura map
typedef struct
{
    int rows;
    int cols;
    unsigned char *cells;
} Map;

// Funkce pro alokaci mapy a její vytvoření ze souboru.
Map *map_ctor(char *filename)
{
    FILE *ptr;
    int check = 0;
    Map *map = malloc(sizeof(Map));
    ptr = fopen(filename, "r");

    if (ptr != NULL && map != NULL)
    {
        if (fscanf(ptr, "%d %d", &map->rows, &map->cols) == 2)
        {
            map->cells = malloc(map->rows * map->cols * sizeof(unsigned char));
            if (map->cells != NULL)
            {
                for (int i = 0; i < map->rows; i++)
                {
                    for (int j = 0; j < map->cols; j++)
                    {
                        if (fscanf(ptr, "%u", (unsigned int *)&map->cells[i * map->cols + j]) == 1)
                        {
                            map->cells[i * map->cols + j] = (unsigned char)map->cells[i * map->cols + j];
                            check++;
                        }
                        else
                        {
                            printf("Chyba načtení jednotlivých hodnot.\n");
                            break;
                        }
                    }
                }
            }
            else
            {
                printf("Chyba alokace paměti.\n");
                fclose(ptr);
                free(map);
            }
        }
    }

    fclose(ptr);
    // printf("%d\n", check);
    if (check == map->cols * map->rows)
        return map;
    else
        return NULL;
}
void map_dtor(Map *map)
{
    free(map->cells);
    free(map);
}
void map_print(Map *map)
{
    if (map != NULL)
    {
        for (int i = 0; i < map->rows; i++)
        {
            for (int j = 0; j < map->cols; j++)
            {
                printf("%hhu ", map->cells[i * map->cols + j]);
            }
            printf("\n");
        }
    }
}
// 2. Podúkol - Kontrola hrany
bool isborder(Map *map, int r, int c, int border)
{
    unsigned char test = map->cells[r * map->cols + c];
    if (border == 1) // Levá hranice
    {
        if (test == 1 || test == 3 || test == 5 || test == 7)
            return true;
    }
    else if (border == 2) // Horní/dolní hranice
    {
        if (test == 4 || test == 5 || test == 6 || test == 7)
            return true;
    }
    else if (border == 3) // Pravá hranice
    {
        if (test == 2 || test == 3 || test == 6 || test == 7)
            return true;
    }
    return false;
}

// 3. Podúkol - Startovní hrana podle podmínek ze zadání
int start_border(Map *map, int r, int c, int leftright)
{
    //  PRAVA RUKA
    if (leftright == 2)
    {
        if (r % 2 == 0 && isborder(map, r, c, 1) == false) // 1
            return 3;

        else if (r % 2 == 1 && isborder(map, r, c, 1) == false) // 2
            return 2;

        else if (((c % 2 == 0 && r % 2 == 0) || (c % 2 == 1 && r % 2 == 1)) && isborder(map, r, c, 3) == false) // 5
            return 2;

        else if (((c % 2 == 0 && r % 2 == 1) || (c % 2 == 1 && r % 2 == 0)) && isborder(map, r, c, 3) == false) // 6
            return 1;

        else if (c % 2 == 0 && r % 2 == 0 && isborder(map, r, c, 2) == false) // 3
            return 1;

        else if (c % 2 == 0 && r % 2 == 1 && isborder(map, r, c, 2) == false) // 4
            return 3;
    }
    // LEVA RUKA
    if (leftright == 1)
    {
        if (r % 2 == 0 && isborder(map, r, c, 1) == false) // 1
            return 2;

        else if (r % 2 == 1 && isborder(map, r, c, 1) == false) // 2
            return 3;

        else if (((c % 2 == 0 && r % 2 == 0) || (c % 2 == 1 && r % 2 == 1)) && isborder(map, r, c, 3) == false) // 5
            return 1;

        else if (((c % 2 == 0 && r % 2 == 1) || (c % 2 == 1 && r % 2 == 0)) && isborder(map, r, c, 3) == false) // 6
            return 2;

        else if (c % 2 == 0 && r % 2 == 0 && isborder(map, r, c, 2) == false) // 3
            return 3;

        else if (c % 2 == 0 && r % 2 == 1 && isborder(map, r, c, 2) == false) // 4
            return 1;
    }
    return 0;
}
int main() // int argc, char *argv[]
{
    Map *map = map_ctor("soubor.txt");
    int leftright = 1;
    map_print(map);
    int border = start_border(map, 6, 2, leftright);
    printf("border=%d", border);
    map_dtor(map);
    return 0;
}