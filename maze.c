/*
Autor: Jakub Smička
Login: xsmickj00
Projekt 2 - bludiště
** Argumenty překladu gcc -std=c11 -Wall -Wextra -Werror maze.c -o maze
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
/*
TODO:
Bonus řešení short test
*/

// Implementace 1. Podúkolu - struktura map
typedef struct
{
    int rows;
    int cols;
    unsigned char *cells;
} Map;

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
// Funkce pro alokaci mapy a její vytvoření ze souboru.
// Funkce vrací vytvořenou strukturu nebo NULL, pokud se cokoliv nepovede (alokace, validace,...)
Map *map_ctor(char *filename)
{
    FILE *ptr;
    int check = 0;
    Map *map = malloc(sizeof(Map));
    ptr = fopen(filename, "r");
    // Zajištění, že je soubor neprázdný a povedla se alokace struktury
    if (ptr != NULL && map != NULL)
    {
        if (fscanf(ptr, "%d %d", &map->rows, &map->cols) == 2) // Načtení počtu řádků a sloupců
        {
            map->cells = malloc(map->rows * map->cols * sizeof(unsigned char)); // Alokace pole
            if (map->cells != NULL)
            {
                for (int i = 0; i < map->rows; i++)
                {
                    for (int j = 0; j < map->cols; j++)
                    {
                        if (fscanf(ptr, "%u", (unsigned int *)&map->cells[i * map->cols + j]) == 1)
                        {
                            if (map->cells[i * map->cols + j] <= 7) // Zajištění, že pole obsahuje číslo 0 - 7
                            {
                                map->cells[i * map->cols + j] = (unsigned char)map->cells[i * map->cols + j];
                                check++;
                            }
                        }
                        else
                        {
                            break; // Při chybě načtení jednotlivých políček
                        }
                    }
                }
            }
            else
            {
                fclose(ptr);
                free(map->cells);
                free(map);
            }
        }
    }
    else
    {
        fclose(ptr);
        free(map);
        return NULL;
    }
    fclose(ptr);

    if (check == map->cols * map->rows) // Finální kontrola správného načtení všech políček a jejich validace
    {
        for (int i = 0; i < map->rows; i++)
        {
            for (int j = 0; j < map->cols - 1; j++)
            {
                if ((isborder(map, i, j, 3) == true && isborder(map, i, j + 1, 1) == false) || (isborder(map, i, j, 3) == false && isborder(map, i, j + 1, 1) == true))
                {
                    free(map->cells);
                    free(map);
                    return NULL;
                }
            }
        }
        for (int i = 0; i < map->rows - 1; i++)
        {
            for (int j = 0; j < map->cols; j++)
            {
                if ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0))
                {
                    if ((isborder(map, i, j, 2) == true && isborder(map, i + 1, j, 2) == false) || (isborder(map, i, j, 2) == false && isborder(map, i + 1, j, 2) == true))
                    {
                        free(map->cells);
                        free(map);
                        return NULL;
                    }
                }
            }
        }
        return map; // Pokud vše projde
    }
    else
    {
        free(map->cells);
        free(map);
        return NULL;
    }
}

// Funkce pro zničení mapy a uvolnění alokované paměti
void map_dtor(Map *map)
{
    map->cols = 0;
    map->rows = 0;
    free(map->cells);
    free(map);
}

// Funkce pro tisk matice (využito při sestavování funkcí)
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

// 3. Podúkol - Startovní hrana podle podmínek ze zadání
// Funkce vrací hodnotu počáteční hranice 1 - Levá, 2 - Spodní/dolní, 3 - Pravá
// Pokud do políčka nešlo vstoupit, tak vrátí 0
int start_border(Map *map, int r, int c, int leftright)
{
    if (r % 2 == 0 && isborder(map, r, c, 1) == false && c == 0)
    {
        if (leftright == 2)
            return 3;
        else
            return 2;
    }
    else if (r % 2 == 1 && isborder(map, r, c, 1) == false && c == 0)
    {
        if (leftright == 2)
            return 2;
        else
            return 3;
    }

    else if (((c % 2 == 0 && r % 2 == 0) || (c % 2 == 1 && r % 2 == 1)) && isborder(map, r, c, 3) == false && c + 1 == map->cols)
    {
        if (leftright == 2)
            return 2;
        else
            return 1;
    }
    else if (((c % 2 == 0 && r % 2 == 1) || (c % 2 == 1 && r % 2 == 0)) && isborder(map, r, c, 3) == false && c + 1 == map->cols)
    {
        if (leftright == 2)
            return 1;
        else
            return 2;
    }
    else if (((c % 2 == 0 && r % 2 == 0) || (c % 2 == 1 && r % 2 == 1)) && isborder(map, r, c, 2) == false && r == 0)
    {
        if (leftright == 2)
            return 1;
        else
            return 3;
    }
    else if (((c % 2 == 1 && r % 2 == 0) || (c % 2 == 0 && r % 2 == 1)) && isborder(map, r, c, 2) == false && r + 1 == map->cols)
    {
        if (leftright == 2)
            return 3;
        else
            return 1;
    }
    return 0;
}

// Vlastní funkce pro hledání cesty.
// Funkce postupně printuje cestu, kudy se prochází bludiště.
// Složité podmínky vznikly postupným bugfixováním při hledání cesty podle zadání.
void map_path(Map *map, int r, int c, int border, int leftright)
{
    bool move = false;
    bool end = false;
    int lastborder = 0;
    int iter = 0;
    while (end == false)
    {
        printf("%d,%d\n", r + 1, c + 1);
        move = false;
        iter++;
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
                        if ((lastborder != 3 && leftright == 1 && isborder(map, r, c, 1) == false) || leftright == 2)
                            border = 1;
                        else
                            border = 3;
                    }
                    else
                    {
                        if (((leftright == 1 && lastborder == 1) && (((c % 2 == 1) && (r % 2 == 0)) || ((c % 2 == 0) && (r % 2 == 1)))) || isborder(map, r, c, 3) == true)
                            border = 1;
                        else
                            border = 3;
                    }
                    continue;
                }
                else if (border == 3)
                {
                    if (((((c % 2 == 1) && (r % 2 == 1)) || ((c % 2 == 0) && (r % 2 == 0))) && leftright == 2) || (leftright == 1 && lastborder != 2 && iter != 1))
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
        if (iter > 1000)
        {
            end = true;
            printf("Algoritmus prekrocil 1000 iteraci - program byl ukoncen.");
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        char str[] = "--help";
        if (strcmp(str, argv[1]) == 0)
        {
            printf("Program lze spustit s nasledujicimi parametry:\n");
            printf("1) ./maze --test nazevsouboru.txt vytiskne Valid, pokud je soubor platny jinak Invalid.\n");
            printf("2) ./maze --rpath R C nazevsouboru.txt vytiskne postupnou cestu bludistem podle pravidla prave ruky.\n");
            printf("3) ./maze --lpath R C nazevsouboru.txt vytiskne postupnou cestu bludistem podle pravidla leve ruky.\n");
            printf("R je radkova a C sloupcova souradnice pocatecniho bodu.\n");
        }
        else
            printf("Invalid syntax.\n");
    }

    else if (argc == 3)
    {
        char str[] = "--test";
        if (strcmp(str, argv[1]) == 0)
        {
            Map *map = map_ctor(argv[2]);
            if (map == NULL)
            {
                printf("Invalid\n");
            }
            else
            {
                printf("Valid\n");
                map_dtor(map);
            }
        }
        else
            printf("Invalid syntax.\n");
    }
    else if (argc == 5)
    {
        char str1[] = "--rpath";
        char str2[] = "--lpath";
        int leftright;
        if (strcmp(str1, argv[1]) == 0)
        {
            leftright = 2;
        }
        else if (strcmp(str2, argv[1]) == 0)
        {
            leftright = 1;
        }
        else
        {
            printf("Invalid syntax.\n");
            return -1;
        }

        Map *map = map_ctor(argv[4]);
        if (map == NULL)
        {
            printf("Chybny soubor nebo je prazdny.\n");
            return -1;
        }
        int r = atoi(argv[2]);
        int c = atoi(argv[3]);
        if (r == 0 || c == 0)
        {
            printf("Chybne zadane souradnice!\n");
            return -1;
        }
        r--;
        c--;

        // map_print(map);
        int border = start_border(map, r, c, leftright);
        if (border == 0)
        {
            printf("Nelze vstoupit na tomto policku!");
            map_dtor(map);
            return -1;
        }
        map_path(map, r, c, border, leftright);
        map_dtor(map);
    }
    else
    {
        printf("Invalid syntax.\n");
        printf("Zkuste spustit './maze --help'\n");
    }
    return 0;
}