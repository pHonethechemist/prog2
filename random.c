#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  srand(time(NULL));
  int d = atoi(argv[1]);
  FILE *f = fopen(argv[2], "w");
  for (int i = 0; i < 2 * d * d; ++i) {
    int r = 0;
    if ((float)rand() / (float)RAND_MAX > 0.5) {
      r = 1;
    }
    fprintf(f, "%d\n", r);
  }
  fclose(f);

  return 0;
}
