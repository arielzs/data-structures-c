#ifndef UTILS_H
#define UTILS_H

int utilsReadInt(const char *prompt);
void utilsClearInputBuffer();
void utilsPause();
void utilsReadLine(const char *prompt, char *buffer, int bufferSize);

#endif