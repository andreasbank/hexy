#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  FILE *fp = NULL;
  char *buffer = NULL;
  size_t buffer_len;
  char *hex = NULL;

  if (argc != 2) return 1;

  if ((fp = fopen(argv[1], "rb")) == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
    return 1;
  }

  fseek(fp, 0L, SEEK_END);
  buffer_len = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  buffer = malloc(buffer_len);

  if (fread(buffer, 1, buffer_len, fp) != buffer_len) {
    fprintf(stderr, "Failed to read the file\n");
    fclose(fp);
    return 1;
  }

  hex = malloc(buffer_len * 2 + 1);

  for (size_t i = 0; i < buffer_len; i++) {
    sprintf(&hex[i * 2], "%02X", buffer[i]);
  }
  hex[buffer_len * 2] = '\0';

  printf("%s\n", hex);

  fclose(fp);
  free(buffer);
  free(hex);

  return 0;
}
