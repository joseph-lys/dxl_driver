#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

bool bufferIsEq(uint8_t* observed, uint8_t* expected, size_t size);
void bufferCopy(uint8_t* src, uint8_t* dst, size_t size);
#endif  // TEST_UTILS_H_
