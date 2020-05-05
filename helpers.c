//
//  helpers.c
//  zprpr2 projekt2
//
//  Created by Jakub Taraba on 05/05/2020.
//  Copyright © 2020 Jakub Taraba. All rights reserved.
//

#include "helpers.h"

int fpeek(FILE * const fp)
{
  const int c = getc(fp);
  return c == EOF ? EOF : ungetc(c, fp);
}

void println(void)
{
    printf("\n");
}

int isnewline(char c) {
    return c == '\n' ? 1 : 0;
}
