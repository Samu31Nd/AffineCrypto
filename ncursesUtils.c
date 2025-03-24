#include "ncursesUtils.h"
#include <ncurses.h>
#include <string.h>
int w, h;

char *programTitle[] = {"     ___           ___         ___                    "
                        " ___           ___    ",
                        "    /  /\\         /  /\\       /  /\\      ___       "
                        "   /__/\\         /  /\\    ",
                        "   /  /::\\       /  /:/_     /  /:/_    /  /\\       "
                        "  \\  \\:\\       /  /:/_   ",
                        "  /  /:/\\:\\     /  /:/ /\\   /  /:/ /\\  /  /:/     "
                        "     \\  \\:\\     /  /:/ /\\  ",
                        " /  /:/~/::\\   /  /:/ /:/  /  /:/ /:/ /__/::\\      "
                        "_____\\__\\:\\   /  /:/ /:/_ ",
                        "/__/:/ /:/\\:\\ /__/:/ /:/  /__/:/ /:/  "
                        "\\__\\/\\:\\__  /__/::::::::\\ /__/:/ /:/ /\\",
                        "\\  \\:\\/:/__\\/ \\  \\:\\/:/   \\  \\:\\/:/      \\ "
                        " \\:\\/\\ \\  \\:\\~~\\~~\\/ \\  \\:\\/:/ /:/",
                        " \\  \\::/       \\  \\::/     \\  \\::/        "
                        "\\__\\::/  \\  \\:\\  ~~~   \\  \\::/ /:/  ",
                        "  \\  \\:\\        \\  \\:\\      \\  \\:\\        "
                        "/__/:/    \\  \\:\\        \\  \\:\\/:/   ",
                        "   \\  \\:\\        \\  \\:\\      \\  \\:\\       "
                        "\\__\\/      \\  \\:\\        \\  \\::/    ",
                        "    \\__\\/         \\__\\/       \\__\\/             "
                        "      \\__\\/         \\__\\/     "};

char *errorTitle[] = {" _______  ______    ______    _______  ______   ",
                      "|       ||    _ |  |    _ |  |       ||    _ |  ",
                      "|    ___||   | ||  |   | ||  |   _   ||   | ||  ",
                      "|   |___ |   |_||_ |   |_||_ |  | |  ||   |_||_ ",
                      "|    ___||    __  ||    __  ||  |_|  ||    __  |",
                      "|   |___ |   |  | ||   |  | ||       ||   |  | |",
                      "|_______||___|  |_||___|  |_||_______||___|  |_|"};

void showErrorMessage(const char *errorMsg) {
  for (int i = TIMEOUT_ERROR_MESSAGE; i > 0; i--) {
    clear();
    for (int i = 0; i < 7; i++) {
      attron(COLOR_PAIR(2));
      mvprintw(2 + i, (w - 48) >> 1, "%s", errorTitle[i]);
      attroff(COLOR_PAIR(2));
    }
    attron(A_BOLD);
    mvprintw((h >> 1), (w - strlen(errorMsg)) >> 1, "%s", errorMsg);
    attroff(A_BOLD);
    mvprintw(h - 3, 2, "Espera %d segundo%s...", i, (i - 1) ? "s" : " ");
    refresh();
    sleep(1);
  }
}

int showResults(int no_args, ...) {
  va_list args;
  va_start(args, no_args);
  clear();
  // print title
  attron(COLOR_PAIR(1));
  for (int i = 0; i < 11; i++) {
    mvprintw(3 + i, (w - 78) >> 1, "%s", programTitle[i]);
  }
  attroff(COLOR_PAIR(1));
  attron(A_BOLD);
  const char *descripcion = va_arg(args, const char *);
  mvprintw(h >> 1, (w - strlen(descripcion)) >> 1, "%s", descripcion);
  attroff(A_BOLD);

  for (int i = 0; i < no_args-1; i++) {
    const char *arg = va_arg(args, const char *);
    mvprintw((h >> 1)+i+1, (w - strlen(arg)) >> 1, "%s", arg);
  }
  refresh();
  getch();
  return 0;
}

void handleError(const int error) {
  char *errMsg;
  switch (error) {
  case 1:
    errMsg = ""; // final del programa
    break;
  case 2:
    errMsg = "Resize the window! Window must be at least 78px of Width";
  }
  endwin();
  fprintf(stderr, "%s\n", errMsg);
  exit(1);
}

void startNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  start_color();
  use_default_colors();

  init_pair(1, COLOR_YELLOW, -1);
  init_pair(2, COLOR_RED, -1);
}

/*
 * returns error code
 * */
int getNumber(const char *description, int *number) {
  int cursor = 0;
  char numberField[255];
  memset(numberField, '\0', sizeof(numberField));
  char charAux = 0;
  do {
    getmaxyx(stdscr, h, w);
    if (w < 80)
      return 2; // error size
    clear();
    box(stdscr, 0, 0);
    // print title
    attron(COLOR_PAIR(1));
    for (int i = 0; i < 11; i++) {
      mvprintw(3 + i, (w - 78) >> 1, "%s", programTitle[i]);
    }
    attroff(COLOR_PAIR(1));
    mvprintw(h - 3, 2, "Press ESC to end program.");
    attron(A_BOLD);
    mvprintw((h >> 1) + 4, (w >> 1) - 5, "%s", description);
    attroff(A_BOLD);
    printNumWindow(numberField, cursor);

    refresh();
    charAux = getch();
    if (isNum(charAux) && cursor < 254) {
      numberField[cursor++] = charAux;
    } else if ((charAux == 7 || charAux == 74) && cursor > 0) {
      numberField[--cursor] = '\0';
    }

  } while (charAux != 27 && charAux != 10 && charAux != 113); // esc
  if (charAux == 10)
    *number = atoi(numberField);

  return (charAux == 10) ? 0 : 1;
}

int isNum(char c) { return (c > 47 && c < 58); }

void printNumWindow(char *content, int length) {
  int centerX, centerY;
  int size = length >> 1;
  centerX = w >> 1;
  centerY = (h >> 1) + (h >> 2);

  mvprintw(centerY - 2, centerX - (size + 2), "+");
  mvprintw(centerY - 2, centerX - (size - length - 2), "+");
  mvprintw(centerY + 2, centerX - (size + 2), "+");
  mvprintw(centerY + 2, centerX - (size - length - 2), "+");
  mvprintw(centerY, centerX - (size - length - 2), "|");

  for (int i = -1; i <= length + 1; i++) {
    mvprintw(centerY - 2, centerX - (size - i), "-");
    mvprintw(centerY + 2, centerX - (size - i), "-");
  }

  mvprintw(centerY, centerX - (size)-2, "| %s", content);
}
