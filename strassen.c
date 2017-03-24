#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N0 60

// contains an array with the matrix's original dimension (od)
typedef struct {
  int *m;
  int od;
} matrix;

// only the top left corner is really needed to specify the location of the
// submatrix. Since these are all square matrices, we only need the dimension to
// finish completely specifying the shape of the submatrix.
typedef struct {
  int toprow, leftcolumn;
} corner;

// a - b = s
// note that it is important that we pass in a matrix type with specified od.
// od is important for indexing into the correct spot.
matrix subtract(matrix a, corner ac, matrix b, corner bc, matrix s, corner sc, int d) {
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      s.m[((i + sc.toprow) * s.od) + j + sc.leftcolumn] = 
        a.m[((i + ac.toprow) * a.od) + j + ac.leftcolumn] -
        b.m[((i + bc.toprow) * b.od) + j + bc.leftcolumn];
    }
  }
  return s;
}

// a and b are the summands, s is the sum
matrix add(matrix a, corner ac, matrix b, corner bc, matrix s, corner sc, int d) {
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      s.m[((i  + sc.toprow) * s.od) + j + sc.leftcolumn] = 
        a.m[((i + ac.toprow) * a.od) + j + ac.leftcolumn] +
        b.m[((i + bc.toprow) * b.od) + j + bc.leftcolumn];
    }
  }
  return s;
}

// a and b are the multiplicands, p is the product which should already have all
// the necessary memory allocated to it.
void normalmultiply(matrix a, corner ac, matrix b, corner bc, matrix p, int d) {
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      int sum = 0;
      for (int k = 0; k < d; ++k) {
        sum += a.m[((i + ac.toprow) * a.od) + k + ac.leftcolumn] * 
          b.m[((k + bc.toprow) * b.od) + j + bc.leftcolumn];
      }
      p.m[(i * p.od) + j] = sum;
    }
  }
}

void strass(matrix a, corner ac, matrix b, corner bc, matrix p, int d, int n0) {
  // in the event that the size of the matrix is odd
  if (d <= n0) {
    normalmultiply(a, ac, b, bc, p, d);
    return;
  }
  if (d % 2 != 0) {
    // copy the original input matrices to new ones of the same size, extend the
    // copies by one row and one column, and set the new row and column to all
    // 0s. then, carry forward with the regular thing. just be sure to disregard
    // the last column and row when writing to p (through add and subtract)
    // because that column and row will be 0 anyway and won't fit into p

    size_t items = (d + 1) * (d + 1);
    size_t size = sizeof(int);

    int *nam = (int *) calloc(items, size),
        *nbm = (int *) calloc(items, size),
        *npm = (int *) calloc(items, size);

    matrix na = {nam, d+1},
           nb = {nbm, d+1},
           np = {npm, d+1};

    // copy a and b to d+1 matrices and pad with zeroes

    for (int i = 0; i < d; ++i) {
      for (int j = 0; j < d; ++j) {
        na.m[(i * (d + 1)) + j] = a.m[((i + ac.toprow) * a.od) + j + ac.leftcolumn];
        nb.m[(i * (d + 1)) + j] = b.m[((i + bc.toprow) * b.od) + j + bc.leftcolumn];
      }
    }

    corner zero = {0, 0};

    strass(na, zero, nb, zero, np, d+1, n0);

    // copy np into p, trimming zeroes

    for (int i = 0; i < d; i++){
      for (int j = 0; j < d; j++){
        p.m[i*d + j] = np.m[i * (d+1) + j];
      }
    }

    free(nam); free(nbm); free(npm);


  } else {
    size_t items = d * d / 4;
    size_t size = sizeof(int);
    size_t ns = items * size;
    int *p1 = (int *) calloc(items, size), *p2 = (int *) calloc(items, size),
        *p3 = (int *) calloc(items, size), *p4 = (int *) calloc(items, size),
        *p5 = (int *) calloc(items, size), *p6 = (int *) calloc(items, size),
        *p7 = (int *) calloc(items, size), *fmh = (int *) malloc(ns),
        *apb = (int *) malloc(ns), *cpd = (int *) malloc(ns),
        *gme = (int *) malloc(ns), *apd = (int *) malloc(ns),
        *eph = (int *) malloc(ns), *bmd = (int *) malloc(ns),
        *gph = (int *) malloc(ns), *amc = (int *) malloc(ns),
        *epf = (int *) malloc(ns);
    int hd = d / 2;
    matrix p1m = {p1, hd}, p2m = {p2, hd}, p3m = {p3, hd}, p4m = {p4, hd},
           p5m = {p5, hd}, p6m = {p6, hd}, p7m = {p7, hd}, fmhm = {fmh, hd},
           apbm = {apb, hd}, cpdm = {cpd, hd}, gmem = {gme, hd},
           apdm = {apd, hd}, ephm = {eph, hd}, bmdm = {bmd, hd},
           gphm = {gph, hd}, amcm = {amc, hd}, epfm = {epf, hd};
    corner topleft = {0, 0}, topright = {0, hd}, bottomleft = {hd, 0},
           bottomright = {hd, hd};
    // atopleft = ac (no need to change this...)
    corner atopright = {ac.toprow, ac.leftcolumn + hd};
    corner abottomleft = {ac.toprow + hd, ac.leftcolumn};
    corner abottomright = {ac.toprow + hd, ac.leftcolumn + hd};
    // btopleft = bc (same)
    corner btopright = {bc.toprow, bc.leftcolumn + hd};
    corner bbottomleft = {bc.toprow + hd, bc.leftcolumn};
    corner bbottomright = {bc.toprow + hd, bc.leftcolumn + hd};

    // calculating the matrices necessary to get the p1 through p7
    subtract(b, btopright, b, bbottomright, fmhm, topleft, hd);
    add(a, ac, a, atopright, apbm, topleft, hd);
    add(a, abottomleft, a, abottomright, cpdm, topleft, hd);
    subtract(b, bbottomleft, b, bc, gmem, topleft, hd);
    add(a, ac, a, abottomright, apdm, topleft, hd);
    add(b, bc, b, bbottomright, ephm, topleft, hd);
    subtract(a, atopright, a, abottomright,  bmdm, topleft, hd);
    add(b, bbottomleft, b, bbottomright, gphm, topleft, hd);
    subtract(a, ac, a, abottomleft, amcm, topleft, hd);
    add(b, bc, b, btopright, epfm, topleft, hd);

    // calculating p1 through p7
    strass(a, ac, fmhm, topleft, p1m, hd, n0);
    strass(apbm, topleft, b, bbottomright, p2m, hd, n0);
    strass(cpdm, topleft, b, bc, p3m, hd, n0);
    strass(a, abottomright, gmem, topleft, p4m, hd, n0);
    strass(apdm, topleft, ephm, topleft, p5m, hd, n0);
    strass(bmdm, topleft, gphm, topleft, p6m, hd, n0);
    strass(amcm, topleft, epfm, topleft, p7m, hd, n0);

    // calculating the four submatrices
    add(p6m, topleft, add(p5m, topleft, subtract(p4m, topleft, p2m, topleft, p, topleft, hd), topleft, p, topleft, hd), topleft, p, topleft, hd);
    add(p1m, topleft, p2m, topleft, p, topright, hd);
    add(p3m, topleft, p4m, topleft, p, bottomleft, hd);
    add(p5m, topleft, subtract(subtract(p1m, topleft, p3m, topleft, p, bottomright, hd), bottomright, p7m, topleft, p, bottomright, hd), bottomright, p, bottomright, hd);


    free(p1); free(p2); free(p3); free(p4); free(p5); free(p6); free(p7);
    free(fmh); free(apb); free(cpd); free(gme); free(apd); free(eph);
    free(bmd); free(gph); free(amc); free(epf);
  }
}

int main(int argc, char *argv[]) {
  // dimension of the matrices we are multiplying
  int d = atoi(argv[2]);
  int *a = (int *) malloc(d * d * sizeof(int)),
      *b = (int *) malloc(d * d * sizeof(int)),
      *s = (int *) malloc(d * d * sizeof(int));

  FILE *f = fopen(argv[3], "r");
  char buf[20];
  // for the first matrix
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      fgets(buf, 11, f);
      a[(i * d) + j] = atoi(buf);
    }
  }
  // for the second matrix
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      fgets(buf, 11, f);
      b[(i * d) + j] = atoi(buf);
    }
  }
  fclose(f);


  corner zero = {0,0};

  matrix am = {a, d},
         bm = {b, d},
         sm = {s, d};


  strass(am, zero, bm, zero, sm, d, N0);
  
  for (int i = 0; i < d; ++i) {
    printf("%d\n", s[(i * d) + i]);
  }

  free(s); free(a); free(b);

  return 0;
}
