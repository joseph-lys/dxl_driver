#include "DxlProtocolV1.h"
#include "gtest/gtest.h"
#include <iostream>

DxlProtocolV1 protocol{};


namespace {


void test_eq_buffer(uint8_t* observed, uint8_t* expected, size_t size) {
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
  EXPECT_FALSE(has_error);
}

static uint8_t buffer[64] = {0};


TEST(V1_Writer_Test, Ping) { 
  uint8_t expected[] = {0xff, 0xff, 0x01, 0x02, 0x01, 0xfb};
  protocol.setTxBuffer(buffer, sizeof(buffer));
  protocol.setTxIns(0x01, DxlProtocolV1::kPingIns);
  protocol.finalizeTx();
  test_eq_buffer(buffer, expected, sizeof(expected));
}

TEST(V1_Writer_Test, Read) { 
  uint8_t expected[] = {0xff, 0xff, 0x01, 0x04, 0x02, 0x2b, 0x01, 0xcc};
  protocol.setTxBuffer(buffer, sizeof(buffer));
  protocol.setTxIns(0x01, DxlProtocolV1::kReadIns);
  protocol.writeTxByte(0x2b);
  protocol.writeTxByte(0x01);
  protocol.finalizeTx();
  test_eq_buffer(buffer, expected, sizeof(expected));
}

TEST(V1_Writer_Test, Write) { 
  uint8_t expected[] = {0xff, 0xff, 0xfe, 0x04, 0x03, 0x03, 0x01, 0xf6};
  protocol.setTxBuffer(buffer, sizeof(buffer));
  protocol.setTxIns(DxlProtocolV1::kBroadcastId, DxlProtocolV1::kWriteIns);
  protocol.writeTxByte(0x03);
  protocol.writeTxByte(0x01);
  protocol.finalizeTx();
  test_eq_buffer(buffer, expected, sizeof(expected));
}

TEST(V1_Writer_Test, Sync_Write) { 
  uint8_t expected[] = {0xff, 0xff, 0xfe, 0x0e, 0x83, 
                        0x1e, 0x04, 0x00, 0x10, 0x00, 
                        0x50, 0x01, 0x01, 0x20, 0x02, 
                        0x60, 0x03, 0x67};
  protocol.setTxBuffer(buffer, sizeof(buffer));
  protocol.setTxIns(DxlProtocolV1::kBroadcastId, DxlProtocolV1::kSyncWriteIns);
  protocol.writeTxByte(0x1E);
  protocol.writeTxByte(0x04);
  protocol.writeTxByte(0x00);
  protocol.writeTxWord(0x0010);
  protocol.writeTxWord(0x0150);
  protocol.writeTxByte(0x01);
  protocol.writeTxWord(0x0220);
  protocol.writeTxWord(0x0360);
  protocol.finalizeTx();
  test_eq_buffer(buffer, expected, sizeof(expected));
}

}
