#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

#define ARRAY_SIZ 1000
using array_mul_func = void (*)(float[ARRAY_SIZ][ARRAY_SIZ], float[ARRAY_SIZ],
                                float[ARRAY_SIZ]);

using array_preprocess_func = void (*)(float[ARRAY_SIZ][ARRAY_SIZ]);

void do_nothing_preprocess(float A[ARRAY_SIZ][ARRAY_SIZ]
                           __attribute__((unused))) {
  return;
}

void naive_array_mul(float A[ARRAY_SIZ][ARRAY_SIZ], float x[ARRAY_SIZ],
                     float y[ARRAY_SIZ]) {
  for (size_t i = 0; i < ARRAY_SIZ; ++i) {
    for (size_t j = 0; j < ARRAY_SIZ; ++j) {
      y[i] += A[i][j] * x[j];
    }
  }
  return;
}

void reversed_idx_array_mul(float A[ARRAY_SIZ][ARRAY_SIZ], float x[ARRAY_SIZ],
                            float y[ARRAY_SIZ]) {
  for (size_t j = 0; j < ARRAY_SIZ; ++j) {
    for (size_t i = 0; i < ARRAY_SIZ; ++i) {
      y[i] += A[i][j] * x[j];
    }
  }
  return;
}

double measure_ms_array_mul(array_mul_func mul_f, array_preprocess_func pre_f,
                            size_t rep_n = 100) {
  float A[ARRAY_SIZ][ARRAY_SIZ];
  float **x = new float *[rep_n];
  float **y = new float *[rep_n];

  // float x[rep_n][ARRAY_SIZ], y[rep_n][ARRAY_SIZ];

  // initialize arrays
  for (size_t i = 0; i < ARRAY_SIZ; ++i) {
    for (size_t j = 0; j < ARRAY_SIZ; ++j) {
      A[i][j] = float(i + j);
    }
  }

  for (size_t i = 0; i < rep_n; ++i) {
    x[i] = new float[ARRAY_SIZ];
    y[i] = new float[ARRAY_SIZ];

    for (size_t j = 0; j < ARRAY_SIZ; ++j) {
      x[i][j] = float(i - j);
      y[i][j] = 0;
    }
  }

  // measurement
  // ----------------------------------------
  std::chrono::system_clock::time_point start, end;
  start = std::chrono::system_clock::now();

  // preprocess
  pre_f(A);

  // calc queue
  for (size_t i = 0; i < rep_n; ++i) {
    mul_f(A, x[i], y[i]);
  }
  end = std::chrono::system_clock::now();
  // ----------------------------------------

  double elapsed_ms_per_rep =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count() /
      1000.0 / rep_n;

  for (size_t i = 0; i < rep_n; ++i) {
    delete[] x[i];
    delete[] y[i];
  }
  delete[] x;
  delete[] y;

  return elapsed_ms_per_rep;
}

#define VERIFY_VAL_RANGE 100.0
#define VERIFY_EPS 1e-7

bool verify_array_mul(array_mul_func mul_f_1, array_preprocess_func pre_f_1,
                      array_mul_func mul_f_2, array_preprocess_func pre_f_2,
                      size_t rep_n = 10) {
  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());

  std::uniform_real_distribution<float> dist(-VERIFY_VAL_RANGE,
                                             VERIFY_VAL_RANGE);

  for (size_t i = 0; i < rep_n; ++i) {
    float A1[ARRAY_SIZ][ARRAY_SIZ], A2[ARRAY_SIZ][ARRAY_SIZ];
    float x[ARRAY_SIZ];
    float y1[ARRAY_SIZ], y2[ARRAY_SIZ];

    // initialize arrays
    for (size_t i = 0; i < ARRAY_SIZ; ++i) {
      for (size_t j = 0; j < ARRAY_SIZ; ++j) {
        float a = dist(engine);
        A1[i][j] = a;
        A2[i][j] = a;
      }
    }
    for (size_t i = 0; i < ARRAY_SIZ; ++i) {
      x[i] = dist(engine);
      y1[i] = 0;
      y2[i] = 0;
    }

    pre_f_1(A1);
    pre_f_2(A2);

    // calc
    mul_f_1(A1, x, y1);
    mul_f_2(A2, x, y2);

    // check
    for (size_t i = 0; i < ARRAY_SIZ; ++i) {
      float difference =
          (y2[i] == 0) ? std::abs(y1[i]) : std::abs(1 - y1[i] / y2[i]);
      bool is_different = difference > VERIFY_EPS;

      // std::cout << "y1[" << i << "]: " << y1[i] << ", y2[" << i
      //           << "]: " << y2[i] << std::endl; // DEBUG:
      if (is_different) {
        return false;
      }
    }
  }
  return true;
}

void show_measurement(std::string title, float ms) {
  std::cout << title << ": " << ms << " msec" << std::endl;
}
void show_verification(std::string title1, std::string title2, bool result) {
  std::cout << title1 << " " << (result ? "===" : "!==") << " " << title2
            << std::endl;
}

int main() {
  show_measurement(
      "native_array_mul",
      measure_ms_array_mul(naive_array_mul, do_nothing_preprocess));

  show_measurement("reversed_array_mul",
                   measure_ms_array_mul(reversed_idx_array_mul,
                                        do_nothing_preprocess));

  show_verification("native_array_mul", "reversed_array_mul",
                    verify_array_mul(naive_array_mul, do_nothing_preprocess,
                                     reversed_idx_array_mul,
                                     do_nothing_preprocess));
  return 0;
}
