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
void add_reg_from_file(options* config, char* filepath);
options arguments_parser(int argc, char* argv[]);
void output(options config, int argc, char* argv[]);
void print_match(regex_t* structure, char* line, char* file_path,
                 options config);
void print_line(char* line, int count);
void processFile(options config, char* path, regex_t* reg);

#endif  // S21_GREP