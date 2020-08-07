#include <emscripten/emscripten.h>
#include <stdio.h>

double EMSCRIPTEN_KEEPALIVE innerProduct(double a[], double b[], int length) {
  double result = 0;
  for (int i = 0; i < length; i++) {
    printf("a1: %d; b1: %d; \n", a[i], b[i]);
    result += a[i] * b[i];
  }
  printf("result: %d; \n", result);
  return result;
}