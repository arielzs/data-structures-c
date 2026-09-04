#include "common.h"
#include "utils.h"

void utilsClearInputBuffer()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* descarta o resto da linha, nao faz nada com o caractere */
    }
}

int utilsReadInt(const char *prompt)
{
    int value;
    int result;

    while (1)
    {
        printf("%s", prompt);
        result = scanf("%d", &value);

        if (result == 1)
        {
            utilsClearInputBuffer();
            return value;
        }

        printf("Entrada invalida! Digite um numero.\n");
        utilsClearInputBuffer();
    }
}

void utilsPause(void)
{
    printf("Pressione ENTER para continuar...");
    getchar();
}