#include "DxlProtocolV1.h"
#include "gtest/gtest.h"
#include <iostream>

DxlWriterV1 writer{};


void test_eq_buffer(uint8_t* observed, uint8_t* expected, size_t size) {
  bool has_error = false;
  std::cout << "observed:" << std::hex << std::endl;
  for (int i=0; i<size; i++) {
    if (i > 0 && i % 8 == 0) { std::cout << std::endl; }
    if (observed[i] != expected[i]) {
      has_error = true;
      std::cout << "*" << observed[i] << "*\t";
    } else {
      std::cout << observed[i] << "\t";
    }
  }
  std::cout << std::endl << "expected:" << std::hex << std::endl;
  for (int i=0; i<size; i++) {
    if (i > 0 && i % 8 == 0) { std::cout << std::endl; }
    std::cout << expected[i] << "\t";
  }
  EXPECT_FALSE(has_error);
}

static uint8_t buffer[64] = {0};


TEST(V1_Writer_Test, Ping) { 
  uint8_t expected[] = {0xff, 0xff, 0x01, 0x02, 0x01, 0xfb};
  writer.setBuffer(buffer, sizeof(buffer));
  writer.setInstruction(0x01, dxl_v1::ping);
  writer.finalize();
  test_eq_buffer(buffer, expected, sizeof(expected));
}

TEST(V1_Writer_Test, Read) { 
  uint8_t expected[] = {0xff, 0xff, 0x01, 0x04, 0x02, 0x2b, 0x01, 0xcc};
  writer.setBuffer(buffer, sizeof(buffer));
  writer.setInstruction(0x01, dxl_v1::read);
  writer.writeByte(0x2b);
  writer.writeByte(0x01);
  writer.finalize();
  test_eq_buffer(buffer, expected, sizeof(expected));
}

TEST(V1_Writer_Test, Write) { 
  uint8_t expected[] = {0xff, 0xff, 0xfe, 0x04, 0x03, 0x03, 0x01, 0xf6};
  writer.setBuffer(buffer, sizeof(buffer));
  writer.setInstruction(dxl_v1::broadcast, dxl_v1::write);
  writer.writeByte(0x03);
  writer.writeByte(0x01);
  writer.finalize();
  test_eq_buffer(buffer, expected, sizeof(expected));
}

TEST(V1_Writer_Test, Sync_Write) { 
  uint8_t expected[] = {0xff, 0xff, 0xfe, 0x0e, 0x83, 
                        0x1e, 0x04, 0x00, 0x10, 0x00, 
                        0x50, 0x01, 0x01, 0x20, 0x02, 
                        0x60, 0x03, 067};
  writer.setInstruction(dxl_v1::broadcast, dxl_v1::sync_write);
  writer.setBuffer(buffer, sizeof(buffer));
  writer.writeByte(0x1E);
  writer.writeByte(0x04);
  writer.writeByte(0x00);
  writer.writeWord(0x0010);
  writer.writeWord(0x0150);
  writer.writeByte(0x01);
  writer.writeWord(0x0220);
  writer.writeWord(0x0360);
  writer.finalize();
  test_eq_buffer(buffer, expected, sizeof(expected));
}

