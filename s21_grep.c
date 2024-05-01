#include "s21_grep.h"

int main(int argc, char* argv[]) {
  options config = arguments_parser(argc, argv);
  output(config, argc, argv);
  return 0;
}

void difine_pattern(options* config, char* pattern) {
  if (config->count_pattern != 0) {
    strcat(config->pattern + config->count_pattern, "|");
    config->count_pattern++;
  }
  config->count_pattern +=
      sprintf(config->pattern + config->count_pattern, "(%s)", pattern);
}

void add_reg_from_file(options* config, char* filepath) {
  FILE* f = fopen(filepath, "r");
  if (f == NULL) {
    if (!config->s) {
      perror(filepath);
      exit(1);
    }
  }
  char* line = NULL;
  size_t memlen = 0;
  int read = getline(&line, &memlen, f);
  while (read != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
      difine_pattern(config, line);
      read = getline(&line, &memlen, f);
    }
    free(line);
  }
}

options arguments_parser(int argc, char* argv[]) {
  options config = {0};
  int num = 0;
  while ((num = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (num) {
      case 'e':
        config.e = 1;
        difine_pattern(&config, optarg);
        break;
      case 'i':
        config.i = REG_ICASE;
        break;
      case 'v':
        config.v = 1;
        break;
      case 'c':
        config.c = 1;
        break;
      case 'l':
        config.l = 1;
        break;
      case 'n':
        config.n = 1;
        break;
      case 'h':
        config.h = 1;
        break;
      case 's':
        config.s = 1;
        break;
      case 'o':
        config.o = 1;
        break;
      case 'f':
        config.f = 1;
        break;
    }
  }
  return config;
}

// void scan_file(FILE* f, char** line, size_t* mem_len, int* len) {
//   if (f == NULL) {
//     perror("ERROR");
//     exit(1);
//   }
//   *len = getline(line, mem_len, f);
// }

void print_line(char* line, int count) {
  for (int i = 0; i < count; i++) {
    putchar(line[i]);
  }
  if (line[count - 1] != '\n') {
    putchar('\n');
  }
}

void print_match(regex_t* structure, char* line) {
  regmatch_t match;
  int offset = 0;
  while (1) {
    int result = regexec(structure, line + offset, 1, &match, 0);
    if (result != 0) {
      break;
    }
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[i]);
    }
    putchar('\n');
    offset == match.rm_eo;
  }
}

void output(options config, int argc, char* argv[]) {
  regex_t structure;
  int value = regcomp(&structure, config.pattern, REG_EXTENDED | config.i);
  if (value) {
    perror("Error");
  }
  for (int i = optind; i < argc; i++) {
    processFile(config, argv[i], &structure);
  }
}
char* line = NULL;
size_t memlen = 0;
int symbol = 0;
while (optind < argc) {
  char* path = argv[optind];
  FILE* f = fopen(path, "r");
  scan_file(f, &line, &memlen, &symbol);
  while (symbol != -1) {
    print_line(line, symbol);
    scan_file(f, &line, &memlen, &symbol);
  }
  optind++;
}
free(line);
}