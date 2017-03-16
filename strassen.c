#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// contains an array with the matrix's original dimension
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
      p.m[(i * b.od) + j] = sum;
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

  } else {
    size_t ns = d * d * sizeof(int) / 4;
    int *p1 = (int *) malloc(ns),
        *p2 = (int *) malloc(ns),
        *p3 = (int *) malloc(ns),
        *p4 = (int *) malloc(ns),
        *p5 = (int *) malloc(ns),
        *p6 = (int *) malloc(ns),
        *p7 = (int *) malloc(ns),
        *fmh = (int *) malloc(ns),
        *apb = (int *) malloc(ns),
        *cpd = (int *) malloc(ns),
        *gme = (int *) malloc(ns),
        *apd = (int *) malloc(ns),
        *eph = (int *) malloc(ns),
        *bmd = (int *) malloc(ns),
        *gph = (int *) malloc(ns),
        *amc = (int *) malloc(ns),
        *epf = (int *) malloc(ns);
    memset(p1, 0, ns); memset(p2, 0, ns); memset(p3, 0, ns); memset(p4, 0, ns);
    memset(p5, 0, ns); memset(p6, 0, ns); memset(p7, 0, ns);
    int hd = d / 2;
    matrix p1m = {p1, hd},
           p2m = {p2, hd},
           p3m = {p3, hd},
           p4m = {p4, hd},
           p5m = {p5, hd},
           p6m = {p6, hd},
           p7m = {p7, hd},
           fmhm = {fmh, hd},
           apbm = {apb, hd},
           cpdm = {cpd, hd},
           gmem = {gme, hd},
           apdm = {apd, hd},
           ephm = {eph, hd},
           bmdm = {bmd, hd},
           gphm = {gph, hd},
           amcm = {amc, hd},
           epfm = {epf, hd};
    corner topleft = {0, 0};
    corner topright = {0, hd};
    corner bottomleft = {hd, 0};
    corner bottomright = {hd, hd};

    // calculating the matrices necessary to get the p1 through p7
    subtract(b, topright, b, bottomright, fmhm, topleft, hd);
    add(a, topleft, a, topright, apbm, topleft, hd);
    add(a, bottomleft, a, bottomright, cpdm, topleft, hd);
    subtract(b, bottomleft, b, topleft, gmem, topleft, hd);
    add(a, topleft, a, bottomright, apdm, topleft, hd);
    add(b, topleft, b, bottomright, ephm, topleft, hd);
    subtract(a, topright, a, bottomright,  bmdm, topleft, hd);
    add(b, bottomleft, b, bottomright, gphm, topleft, hd);
    subtract(a, topleft, a, bottomleft, amcm, topleft, hd);
    add(b, topleft, b, topright, epfm, topleft, hd);
    /* 
    printf("%d\n", fmhm.m[0]);
    printf("%d\n", apbm.m[0]);
    printf("%d\n", cpdm.m[0]);
    printf("%d\n", gmem.m[0]);
    printf("%d\n", apdm.m[0]);
    printf("%d\n", ephm.m[0]);
    printf("%d\n", bmdm.m[0]);
    printf("%d\n", gphm.m[0]);
    printf("%d\n", amcm.m[0]);
    printf("%d\n", epfm.m[0]);
    */ 


    // calculating p1 through p7
    strass(a, topleft, fmhm, topleft, p1m, hd, n0);
    strass(apbm, topleft, b, bottomright, p2m, hd, n0);
    strass(cpdm, topleft, b, topleft, p3m, hd, n0);
    strass(a, bottomright, gmem, topleft, p4m, hd, n0);
    strass(apdm, topleft, ephm, topleft, p5m, hd, n0);
    strass(bmdm, topleft, gphm, topleft, p6m, hd, n0);
    strass(amcm, topleft, epfm, topleft, p7m, hd, n0);
    /*
    printf("%d\n", p1[0]);
    printf("%d\n", p2[0]);
    printf("%d\n", p3[0]);
    printf("%d\n", p4[0]);
    printf("%d\n", p5[0]);
    printf("%d\n", p6[0]);
    printf("%d\n", p7[0]);
    */

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
  char buf[11];
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

  memset(s, 0, d * d * sizeof(int));

  corner zero = {0,0};

  matrix am = {a, d},
         bm = {b, d},
         sm = {s, d};

  strass(am, zero, bm, zero, sm, d, 1);

  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      printf("%d\n", s[(i * d) + j]);
    }
  }

  free(s); free(a); free(b);

  return 0;
}
