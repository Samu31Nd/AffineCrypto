#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#define TIMEOUT_ERROR_MESSAGE 3

int isNum(char c);
void printNumWindow(char *num, int cursor);
int getNumber(const char*, int*);
void startNcurses();
void handleError(const int);
void showErrorMessage(const char*);
int showResults(int no_args,...);
