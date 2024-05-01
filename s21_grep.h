#ifndef S21_GREP
#define S21_GREP

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int count_pattern;
  char pattern[1024];
} options;

void difine_pattern(options* config, char* pattern);
options arguments_parser(int argc, char* argv[]);
void output(options config, int argc, char* argv[]);
void scan_file(FILE* f, char** line, size_t* mem_len, int* len);
void print_line(char* line, int count);

#endif  // S21_GREP