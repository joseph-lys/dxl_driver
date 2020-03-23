#include "DxlProtocolV1.h"
#include "gtest/gtest.h"
#include <iostream>
#include "utils.h"

namespace {


DxlProtocolV1 protocol{};
static uint8_t buffer[64] = {0};


TEST(V1_Writer_Test, Ping) { 
  uint8_t expected[] = {0xff, 0xff, 0x01, 0x02, 0x01, 0xfb};
  protocol.setTxBuffer(buffer, sizeof(buffer));
  protocol.initTxData(0x01, DxlProtocolV1::kPing);
  protocol.finalizeTx();
  EXPECT_TRUE(bufferIsEq(buffer, expected, sizeof(expected)));
}

TEST(V1_Writer_Test, Read) { 
  uint8_t expected[] = {0xff, 0xff, 0x01, 0x04, 0x02, 0x2b, 0x01, 0xcc};
  protocol.setTxBuffer(buffer, sizeof(buffer));
  protocol.initTxData(0x01, DxlProtocolV1::kRead);
  protocol.writeTxByte(0x2b);
  protocol.writeTxByte(0x01);
  protocol.finalizeTx();
  EXPECT_TRUE(bufferIsEq(buffer, expected, sizeof(expected)));
}

TEST(V1_Writer_Test, Write) { 
  uint8_t expected[] = {0xff, 0xff, 0xfe, 0x04, 0x03, 0x03, 0x01, 0xf6};
  protocol.setTxBuffer(buffer, sizeof(buffer));
  protocol.initTxData(DxlProtocolV1::kBroadcastId, DxlProtocolV1::kWrite);
  protocol.writeTxByte(0x03);
  protocol.writeTxByte(0x01);
  protocol.finalizeTx();
  EXPECT_TRUE(bufferIsEq(buffer, expected, sizeof(expected)));
}

TEST(V1_Writer_Test, Sync_Write) { 
  uint8_t expected[] = {0xff, 0xff, 0xfe, 0x0e, 0x83, 
                        0x1e, 0x04, 0x00, 0x10, 0x00, 
                        0x50, 0x01, 0x01, 0x20, 0x02, 
                        0x60, 0x03, 0x67};
  protocol.setTxBuffer(buffer, sizeof(buffer));
  protocol.initTxData(DxlProtocolV1::kBroadcastId, DxlProtocolV1::kSyncWrite);
  protocol.writeTxByte(0x1E);
  protocol.writeTxByte(0x04);
  protocol.writeTxByte(0x00);
  protocol.writeTxWord(0x0010);
  protocol.writeTxWord(0x0150);
  protocol.writeTxByte(0x01);
  protocol.writeTxWord(0x0220);
  protocol.writeTxWord(0x0360);
  protocol.finalizeTx();
  EXPECT_TRUE(bufferIsEq(buffer, expected, sizeof(expected)));
}

}
