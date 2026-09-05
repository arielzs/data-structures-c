#include "common.h"
#include "utils.h"

void utilsClearInputBuffer(void)
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

        if (result == EOF)
        {
            printf("\nEntrada encerrada. Finalizando o programa.\n");
            exit(0);
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

void utilsReadLine(const char *prompt, char *buffer, int bufferSize)
{
    printf("%s", prompt);

    if (fgets(buffer, bufferSize, stdin) != NULL)
    {
        int length = strlen(buffer);

        if (length > 0 && buffer[length - 1] == '\n')
        {
            buffer[length - 1] = '\0';
        }
    }
    else
    {
        buffer[0] = '\0';
    }
}