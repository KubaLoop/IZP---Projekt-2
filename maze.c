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
    if (check == map->cols * map->rows)
        return map;
    else
        return NULL;
}
// Funkce pro zničení mapy a uvolnění alokované paměti
void map_dtor(Map *map)
{
    free(map->cells);
    free(map);
}
// Funkce pro tisk matice
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
// Funkce vrací hodnotu počáteční hranice 1 - Levá, 2 - Spodní/dolní, 3 - Pravá
int start_border(Map *map, int r, int c, int leftright)
{
    if (r % 2 == 0 && isborder(map, r, c, 1) == false)
    {
        if (leftright == 2)
            return 3;
        else
            return 2;
    }
    else if (r % 2 == 1 && isborder(map, r, c, 1) == false)
    {
        if (leftright == 2)
            return 2;
        else
            return 3;
    }

    else if (((c % 2 == 0 && r % 2 == 0) || (c % 2 == 1 && r % 2 == 1)) && isborder(map, r, c, 3) == false)
    {
        if (leftright == 2)
            return 2;
        else
            return 1;
    }
    else if (((c % 2 == 0 && r % 2 == 1) || (c % 2 == 1 && r % 2 == 0)) && isborder(map, r, c, 3) == false)
    {
        if (leftright == 2)
            return 1;
        else
            return 2;
    }
    else if (c % 2 == 0 && r % 2 == 0 && isborder(map, r, c, 2) == false)
    {
        if (leftright == 2)
            return 1;
        else
            return 3;
    }
    else if (c % 2 == 0 && r % 2 == 1 && isborder(map, r, c, 2) == false)
    {
        if (leftright == 2)
            return 3;
        else
            return 1;
    }
    return 0;
}

// Vlastní funkce pro hledání cesty.
// Složité podmínky vznikly postupným bugfixováním při hledání cesty podle zadání.
void map_path(Map *map, int r, int c, int border, int leftright)
{
    bool move = false;
    bool end = false;
    int lastborder = 0;
    while (end == false)
    {
        printf("%d,%d\n", r + 1, c + 1);
        move = false;
        while (move == false)
        {
            if (isborder(map, r, c, border) == false)
            {
                if (border == 1)
                {
                    c--;
                    lastborder = border;
                    if ((((c % 2 == 1) && (r % 2 == 1)) || ((c % 2 == 0) && (r % 2 == 0))) && leftright == 2)
                        border = 2;
                    else if (((((c % 2 == 0) && (r % 2 == 1)) || ((c % 2 == 1) && (r % 2 == 0))) && leftright == 1))
                        border = 2;
                    move = true;
                }
                else if (border == 3)
                {
                    c++;
                    lastborder = border;
                    if (leftright == 2)
                    {
                        if (((c % 2 == 1) && (r % 2 == 1)) || ((c % 2 == 0) && (r % 2 == 0)))
                            border = 3;
                        else
                            border = 2;
                    }
                    else if (leftright == 1)
                    {
                        if (((c % 2 == 1) && (r % 2 == 1)) || ((c % 2 == 0) && (r % 2 == 0)))
                            border = 2;
                        else
                            border = 3;
                    }
                    move = true;
                }
                else if (border == 2 && (((c % 2 == 0 && r % 2 == 1) || (c % 2 == 1 && r % 2 == 0))))
                {
                    r++;
                    lastborder = border;
                    if (leftright == 2)
                        border = 1;
                    else
                        border = 3;
                    move = true;
                }
                else if (border == 2 && ((c % 2 == 0 && r % 2 == 0) || (c % 2 == 1 && r % 2 == 1)))
                {
                    r--;
                    lastborder = border;
                    if (leftright == 2)
                        border = 3;
                    else
                        border = 1;
                    move = true;
                }
            }
            else
            {
                if (border == 1)
                {
                    if (((((c % 2 == 1) && (r % 2 == 1)) || ((c % 2 == 0) && (r % 2 == 0))) && leftright == 2) || (leftright == 1 && (((c % 2 == 0) && (r % 2 == 1)) || ((c % 2 == 1) && (r % 2 == 0)))))
                        border = 3;
                    else
                        border = 2;
                    continue;
                }
                else if (border == 2)
                {
                    if (((c % 2 == 1) && (r % 2 == 1)) || ((c % 2 == 0) && (r % 2 == 0)))
                    {
                        if ((lastborder != 3 && leftright == 1) || leftright == 2)
                            border = 1;
                        else
                            border = 3;
                    }
                    else
                    {
                        if ((leftright == 1 && lastborder == 1) && (((c % 2 == 1) && (r % 2 == 0)) || ((c % 2 == 0) && (r % 2 == 1))))
                            border = 1;
                        else
                            border = 3;
                    }
                    continue;
                }
                else if (border == 3)
                {
                    if (((((c % 2 == 1) && (r % 2 == 1)) || ((c % 2 == 0) && (r % 2 == 0))) && leftright == 2) || (leftright == 1 && lastborder != 2))
                        border = 2;
                    else
                        border = 1;
                    lastborder = 0;
                    continue;
                }
            }
        }
        // Vyhodnocení, zda jsme nevyjeli mimo mapu == konec
        if (c > map->cols - 1 || c < 0 || r > map->rows - 1 || r < 0)
        {
            end = true;
        }
    }
}
int main() // int argc, char *argv[]
{
    Map *map = map_ctor("soubor.txt");
    if (map == NULL)
        return -1;

    int leftright = 2;

    map_print(map);

    int border = start_border(map, 5, 0, leftright);
    map_path(map, 5, 0, border, leftright);
    map_dtor(map);

    return 0;
}
