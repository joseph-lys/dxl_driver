#include <iostream>
#include <stdio.h>

bool bufferIsEq(uint8_t* observed, uint8_t* expected, size_t size) {
  bool has_error = false;
  for (int i=0; i<size; i++) {
    if (observed[i] != expected[i]) {
      has_error = true;
    }
  }
  if (has_error) {  // print debug information only if there is an error
    std::cout << "observed:" << std::hex << std::endl;
    for (int i=0; i<size; i++) {
      if (i > 0 && i % 8 == 0) { std::cout << std::endl; }
      if (observed[i] != expected[i]) {
        std::cout << "*" << static_cast<int>(observed[i]) << "*\t";
      } else {
        std::cout << static_cast<int>(observed[i]) << "\t";
      }
    }
    std::cout << std::endl << "expected:" << std::hex << std::endl;
    for (int i=0; i<size; i++) {
      if (i > 0 && i % 8 == 0) { std::cout << std::endl; }
      std::cout << static_cast<int>(expected[i]) << "\t";
    }
  }
  return ~has_error;
}

void bufferCopy(uint8_t* src, uint8_t* dst, size_t size) {
  // memcpy(static_cast<void*>(dst), static_cast<void*>(src), size);
}