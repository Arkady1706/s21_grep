#include "s21_grep.h"

int main(int argc, char* argv[]) {
  options config = arguments_parser(argc, argv);
  output(config, argc, argv);
  return 0;
}

void difine_pattern(options* config, char* pattern) {
  // int n = strlen(pattern);
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
    }
    exit(1);
  }
  char* line = NULL;
  size_t memlen = 0;
  int read = getline(&line, &memlen, f);
  while (read != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    difine_pattern(config, line);
    read = getline(&line, &memlen, f);
  }
  free(line);
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
        add_reg_from_file(&config, optarg);
        break;
    }
  }
  if (config.count_pattern == 0) {
    difine_pattern(&config, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) {
    config.f = 1;
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

void print_match(regex_t* structure, char* line, char* file_path,
                 options config) {
  regmatch_t match;
  int flag = 0;
  //  char* duplicate = strdup(line);
  //  if (duplicate == NULL) {
  //        perror("ERROR");
  //        exit(1);
  //    }
  while (1) {
    int result = regexec(structure, line, 1, &match, 0);
    int end = match.rm_eo;
    if (result != 0) {
      break;
    }
    if (flag > 0 && !config.h) {
      printf("%s:", file_path);
    }
    flag++;
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[i]);
    }
    putchar('\n');
    line = line + end + 1;
  }
}

void processFile(options config, char* path, regex_t* reg) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    if (!config.s) {
      perror(path);
    }
    exit(1);
  }
  char* line = NULL;
  size_t memlen = 0;
  int read = 0;
  int line_count = 1;
  int c = 0;
  read = getline(&line, &memlen, f);
  while (read != -1) {
    int result = regexec(reg, line, 0, NULL, 0);
    if ((result == 0 && !config.v) || (config.v && result != 0)) {
      if (!config.c && !config.l) {
        if (!config.h) {
          printf("%s:", path);
        }
        if (config.n) {
          printf("%d:", line_count);
        }
        if (config.o) {
          print_match(reg, line, path, config);
        } else {
          print_line(line, read);
        }
      }
      c++;
    }
    read = getline(&line, &memlen, f);
    line_count++;
  }
  free(line);
  if (config.c && !config.l) {
    if (!config.h) {
      printf("%s:", path);
    }
    printf("%d\n", c);
  }
  if (config.l && c > 0) {
    printf("%s\n", path);
  }
  fclose(f);
}

void output(options config, int argc, char* argv[]) {
  regex_t structure;
  // printf("%s", config.pattern);
  int value = regcomp(&structure, config.pattern, REG_EXTENDED | config.i);
  if (value) {
    perror("Error");
  }
  for (int i = optind; i < argc; i++) {
    processFile(config, argv[i], &structure);
  }
  regfree(&structure);
}

// while (optind < argc) {
//   char* path = argv[optind];
//   FILE* f = fopen(path, "r");
//   scan_file(f, &line, &memlen, &symbol);
//   while (symbol != -1) {
//     print_line(line, symbol);
//     scan_file(f, &line, &memlen, &symbol);
//   }
//   optind++;
// }
// free(line);
// }