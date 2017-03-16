#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// a - b = s
void subtract(int *a, int *b, int *s, int d) {
  for (int i = 0; i < d; ++i)
    for (int j = 0; j < d; ++j)
      s[(i * d) + j] = a[(i * d) + j] - b[(i * d) + j];
}

// a and b are the summands, s is the sum
void add(int *a, int *b, int *s, int d) {
  for (int i = 0; i < d; ++i)
    for (int j = 0; j < d; ++j)
      s[(i * d) + j] = a[(i * d) + j] + b[(i * d) + j];
}

// a and b are the multiplicands, p is the product which should already have all
// the necessary memory allocated to it.
void normalmultiply(int *a, int *b, int *p, int d) {
  for (int i = 0; i < d; ++i)
    for (int j = 0; j < d; ++j) {
      int sum = 0;
      for (int k = 0; k < d; ++k)
        sum += a[(i * d) + k] * b[(k * d) + j];
      p[(i * d) + j] = sum;
    }
}

void strass(int *a, int *b, int *p, int d) {

}

int main(int argc, char *argv[]) {
  // dimension of the matrices we are multiplying
  int d = atoi(argv[2]);
  int *a = (int *) malloc(d * d * sizeof(int));
  int *b = (int *) malloc(d * d * sizeof(int));
  FILE *f = fopen(argv[3], "r");
  char buf[11];
  // for the first matrix
  for (int i = 0; i < d; ++i)
    for (int j = 0; j < d; ++j) {
      fgets(buf, 11, f);
      a[(i * d) + j] = atoi(buf);
    }
  // for the second matrix
  for (int i = 0; i < d; ++i)
    for (int j = 0; j < d; ++j) {
      fgets(buf, 11, f);
      b[(i * d) + j] = atoi(buf);
    }

  // a little test to make sure that add works
  int *s = (int *) malloc(d * d * sizeof(int));
  
  add(a, b, s, d);
  for (int i = 0; i < d; ++i)
    for (int j = 0; j < d; ++j)
      printf("%d\n", s[(i * d) + j]);
      

  // a little test to make sure that subtract works
  subtract(a, b, s, d);
  for (int i = 0; i < d; ++i)
    for (int j = 0; j < d; ++j)
      printf("%d\n", s[(i * d) + j]);

  // a little test to make sure that normalmultiply works
  normalmultiply(a, b, s, d);
  for (int i = 0; i < d; ++i)
    for (int j = 0; j < d; ++j)
      printf("%d\n", s[(i * d) + j]);



  // when we want to split arrays, it'll be easiest to pass in just the 4
  // corners of the matrix we want to work with using the indices of the
  // enclosing matrix. For example, if we want to split up a 100x100 matrix, we
  // should specify the box with columns 0 -> 50 and rows 0 -> 50. If we want to
  // specify the bottom right corner, we specify columns 50 -> 100 and rows 50
  // -> 100. for looping purposes, it will be easiest to make the left end-point
  // of the interval inclusive and the right point exclusive. 
  // Thus, the helper functions written should also accept a bunch of additional
  // arguments which will have to do with the regions inside the three matrices:
  // a few indices to understand the region in the left matrix, a few for the
  // right, and a few for the resulting matrix (whether it be product, sum, or
  // difference).
  free(s);
  free(a);
  free(b);

  return 0;
}
