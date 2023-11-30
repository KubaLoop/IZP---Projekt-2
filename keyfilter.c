/**********************************************************************************************************************************
Autor: Jakub Smička
Login: xmickj00
Last change: 26. 10. 2023
Název: keyfilter.c
Popis programu: Program má na základě vstupu hledat města v textovém souboru, který se otvírá na stdin.
                Program má 3 různé stavy - Not found, Found "město" a Enable, kdy vypíše další znaky, které vedou k jednomu z měst.
***********************************************************************************************************************************/
// Preklad co mame pouzit gcc -std=c11 -Wall -Wextra -Werror keyfilter.c -o keyfilter

#include <stdio.h>
#include <string.h>
#define MAX 102

char makeupper(char lwr)
{
    if (lwr >= 97 && lwr <= 122) // Předělání všech malých písmen na velká
    {
        lwr -= 32;
    }
    return lwr;
}

void format(char *enabled, int n)
{
    int index = 0;
    // Zabezpečení proti duplikovaným písmenům.
    for (int i = 0; i < n; i++)
    {
        int j;
        for (j = 0; j < index; j++)
            if (enabled[i] == enabled[j])
                break;
        if (j == index)
            enabled[index++] = enabled[i];
    }

    enabled[index] = '\0'; // Zajištění, že se eliminací stejných znaků zmenšilo pole.

    // Seřazení podle abecedy.
    for (int i = 0; i < index - 1; i++)
    {
        for (int j = 0; j < index - i - 1; j++)
        {
            if (enabled[j] > enabled[j + 1])
            {
                char temp = enabled[j];
                enabled[j] = enabled[j + 1];
                enabled[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    char str[MAX];
    int len = 1;

    if (argv[1] != NULL)
        len = strlen(argv[1]);

    int len2;
    int check;
    int founded = 0;
    char enabled[MAX] = "";
    char fndtwn[MAX] = "";
    int enb = 0;
    char town[MAX];
    int townlen;
    char temp;
    char temp2;

    while (fgets(str, MAX, stdin) != NULL) // Čtení souboru na stdin
    {
        // printf("%s", str);
        check = 0;
        len2 = strlen(str);
        for (int i = 0; i < len + 1; i++) // Procházení jednoho města
        {
            temp = makeupper(str[i]);
            if (argc < 2) // Zajištění, že nenastane seg. fault
            {
                enabled[enb] = temp;
                enb++;
                founded++;
                break; // Nemusíme dál procházet město, bereme rovnou první písmeno od každého města
            }
            temp2 = makeupper(argv[1][i]);
            if (temp == temp2)
            {
                check++;
            }
            else
            {
                if (check == len)
                {
                    enabled[enb] = temp;
                    enb++;
                    founded++;
                    strcpy(town, str); // Pokud se najde jen 1 město, tak se tato proměnná použije pro tisk FOUND
                    townlen = len2 - 1;
                }
                continue;
            }
        }
    }

    // Nenašlo se žádné město
    if (founded == 0)
        fprintf(stdout, "Not found\n");
    // Našlo se právě jedno město
    else if (founded == 1)
    {
        for (int i = 0; i < townlen; i++)
        {
            temp = makeupper(town[i]);
            fndtwn[i] = temp;
        }
        fprintf(stdout, "Found: %s\n", fndtwn);
    }
    // Našlo se více měst
    else if (founded > 1)
    {
        int n = strlen(enabled);
        format(enabled, n); // Volání vlastní funkce pro eliminaci stejných znaků a seřazení podle abecedy
        fprintf(stdout, "Enable: %s\n", enabled);
    }

    return 0;
}