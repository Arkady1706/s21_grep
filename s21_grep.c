#include "s21_grep.h"

int main(int argc, char* argv[]) {
  options config = arguments_parser(argc, argv);
  output(config, argc, argv);
  return 0;
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
      case '?':
        perror("ERROR");
        exit(1);
        break;
      default:
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
    }
    exit(1);
  }
  char* line = NULL;
  size_t max = 0;
  int read = getline(&line, &max, f);
  while (read != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    difine_pattern(config, line);
    read = getline(&line, &max, f);
  }
  free(line);
}

void print_line(char* line, int count) {
  for (int i = 0; i < count; i++) {
    putchar(line[i]);
  }
  if (line[count - 1] != '\n') {
    putchar('\n');
  }
}

void print_match(regex_t* structure, char* line, char* file_path,
                 options config, int count) {
  regmatch_t match;
  int flag = 0;
  int shift = 0;
  while (1) {
    int result = regexec(structure, line + shift, 1, &match, 0);
    int end = match.rm_eo;
    if (result != 0) {
      break;
    }
    if (flag > 0 && !config.h) {
      printf("%s:", file_path);
    }
    if (flag > 0 && config.n) {
      printf("%d:", count);
    }
    flag++;
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[shift + i]);
    }
    putchar('\n');
    shift += end;
  }
}

void processFile(options config, char* path, regex_t* reg) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    if (!config.s) perror(path);
    return;
  }
  char* line = NULL;
  size_t max = 0;
  int read = 0;
  int line_count = 1;
  int n_line = 0;
  read = getline(&line, &max, f);
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
          print_match(reg, line, path, config, line_count);
        } else {
          print_line(line, read);
        }
      }
      n_line++;
    }
    read = getline(&line, &max, f);
    line_count++;
  }
  free(line);
  if (config.c && config.l) {
    if (!config.h) printf("%s:", path);
  }
  if (!config.l && config.c) {
    if (!config.h) printf("%s:", path);
    printf("%d\n", n_line);
  }
  if (config.l && n_line > 0) printf("%s\n", path);
  fclose(f);
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
  regfree(&structure);
}