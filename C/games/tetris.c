/* -*- coding:utf-8 -*-
 *
 * Copyright (C) 2018 Ken'ichi Fukamachi <fukachan@fml.org>
 *          All right reserved.
 *
 * you can redistribute it and/or modify
 * it under the terms of GNU General Public License v3.
 * See the file ../../LICENSE for more details.
 *
 * 【C言語の発展課題用】テトリスのテンプレート
 *    上から横棒が落ちてきて、q を入力すると終了します。
 *    現状それだけですが、入力とかタイムアウトについてヒントになるでしょう。
 *
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#else
#include <term.h>
#endif
#include <termios.h>
#include <unistd.h>
#include <string.h>

void stop (char *msg);
void console_init (void);
void console_recover (void);
void console_clear (void);
char input (void);
void block_show (void);


static struct termios oldtt, newtt;
static int baseline;


void stop (char *msg) {
  console_recover();
  fprintf(stderr, "%s\n", msg);
  exit(1);
}


// 端末をロー(raw)モード、read()のタイムアウトを 0.5秒に設定
void console_init (void) {
  if (tcgetattr(0, &oldtt) < 0)
    stop("tcgetattr() fails");
  newtt = oldtt;
  newtt.c_lflag &= ~(ICANON|ECHO);
  newtt.c_cc[VMIN]  = 0;
  newtt.c_cc[VTIME] = 5;
  if (tcsetattr(0, TCSANOW, &newtt) < 0)
    stop("tcsetattr() fails");
}


// 端末を、ローモードから元のモードへ戻す
void console_recover (void) {
  if (tcsetattr(0, TCSANOW, &oldtt) < 0)
    stop("tcsetattr() fails");
}


void console_clear (void) {
  system("/usr/bin/clear");
}


// read 1 byte, return it.
char input (void) {
  char c;
  
  read(0, &c, 1);
  // fprintf(stderr, "(debug) read = %c\n", c);

  return c;
}


void block_show (void) {
  int i, j;

  baseline++;
  baseline = baseline % 20;
  for (i = 0; i < 20; i++) {
    if (baseline == i) {
      for (j = 0; j < 10; j++) {
	fprintf(stderr, "□");
      }
      // fprintf(stderr, " (debug baseline = %i)\n", baseline);
      fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
  }
}


int main (int argc, char **argv, char **env) {
  char c;

  baseline = 0;
  console_init();
  while (1) {
    block_show();
    c = input();
    if (strcmp(&c, "q") == 0) {
      break;
    }
    console_clear();
  }
  console_recover();
  exit(0);
}
