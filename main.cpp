#include <iostream>
#include <time.h>

#define ARRAY_SIZ 1000
using array_mul_func = void (*)(float[ARRAY_SIZ][ARRAY_SIZ], float[ARRAY_SIZ],
                                float[ARRAY_SIZ]);

void naive_array_mul(float A[ARRAY_SIZ][ARRAY_SIZ], float x[ARRAY_SIZ],
                     float y[ARRAY_SIZ]) {
  // TODO:
  return;
}

#define MEASURE_REP 100
double measure_ms_array_mul(array_mul_func f) {
  float A[ARRAY_SIZ][ARRAY_SIZ];
  float x[MEASURE_REP][ARRAY_SIZ], y[MEASURE_REP][ARRAY_SIZ];

  // initialize arrays
  for (int i = 0; i < ARRAY_SIZ; ++i) {
    for (int j = 0; j < ARRAY_SIZ; ++j) {
      A[i][j] = float(i + j);
    }
  }
  for (int i = 0; i < MEASURE_REP; ++i) {
    for (int j = 0; j < ARRAY_SIZ; ++j) {
      x[i][j] = float(i - j);
      y[i][j] = 0;
    }
  }

  // measurement
  clock_t start = clock();
  for (auto i = 0; i < MEASURE_REP; ++i) {
    f(A,x[i],y[i]);
  }
  clock_t end = clock();
  
  return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
}

int main() { measure_ms_array_mul(naive_array_mul); }
