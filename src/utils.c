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

        if (result == EOF)
        {
            printf("\nInput closed. Exiting the program.\n");
            exit(0);
        }

        printf("Invalid input! Enter a number.\n");
        utilsClearInputBuffer();
    }
}

void utilsPause()
{
    printf("Press ENTER to continue...");
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