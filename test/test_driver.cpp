#include "DxlDriver.h"
#include "DxlProtocolV1.h"
#include "mock.h"
#include "gtest/gtest.h"
#include <iostream>
#include "utils.h"

namespace {


TEST(Driver_Test, UsecaseV1_Ping) { 
  uint8_t tx_data[] = {0xff, 0xff, 0x01, 0x02, 0x01, 0xfb};
  uint8_t rx_data[] = {0xff, 0xff, 0x01, 0x02, 0x00, 0xfc};
  MockHwDriver mock_hw{};
  DxlDriver driver = DxlDriver::create<DxlProtocolV1, 128, 128>(mock_hw);
  driver.setTxIns(0x01, DxlProtocolV1::Ins::kPing);
  driver.beginTransmission();
  EXPECT_EQ(driver.poll(), DxlDriver::kTransmitting);
  EXPECT_EQ(mock_hw.mTxBufSize, sizeof(tx_data));
  EXPECT_TRUE(bufferIsEq(mock_hw.pTxBuf, tx_data, sizeof(tx_data)));

  mock_hw.mTxDone = true;
  EXPECT_EQ(driver.poll(), DxlDriver::kReceiving);

  mock_hw.mockReceive(rx_data, sizeof(rx_data) - 1);
  EXPECT_EQ(driver.poll(), DxlDriver::kReceiving);

  mock_hw.mockReceive(rx_data[sizeof(rx_data) - 1]);
  EXPECT_EQ(driver.poll(), DxlDriver::kDone);
  
  EXPECT_EQ(driver.getRxId(), 0x01);
  EXPECT_EQ(driver.getRxStatusByte(), 0);  
}

TEST(Driver_Test, UsecaseV1_Write) {
  uint8_t tx_data[] = {0xff, 0xff, 0xFE, 0x04, 0x03, 0x03, 0x01, 0xf6};
  uint8_t rx_data[] = {0xff, 0xff, 0x01, 0x02, 0x00, 0xfc};
  MockHwDriver mock_hw{};
  DxlDriver driver = DxlDriver::create<DxlProtocolV1, 128, 128>(mock_hw);
  
  driver.setTxIns(DxlProtocolV1::Ins::kBroadcastId, DxlProtocolV1::Ins::kWrite);
  driver.writeTxByte(0x03);
  driver.writeTxByte(0x01);
  driver.beginTransmission();
  EXPECT_EQ(mock_hw.mTxBufSize, sizeof(tx_data));
  EXPECT_TRUE(bufferIsEq(mock_hw.pTxBuf, tx_data, sizeof(tx_data)));
  EXPECT_EQ(driver.poll(), DxlDriver::kTransmitting);
  
  mock_hw.mTxDone = true;
  EXPECT_EQ(driver.poll(), DxlDriver::kReceiving);

  // when timeout is reached
  mock_hw.mTimeout = true;  
  EXPECT_EQ(driver.poll(), DxlDriver::kDone);
  EXPECT_EQ(driver.getRxId(), 0xff);
  EXPECT_EQ(driver.getRxStatusByte(), 0xff);  
  EXPECT_EQ(driver.readRxByte(), 0xff);  // no more data, should get 0xff    
  EXPECT_EQ(driver.readRxWord(), 0xffff);  // not enough data, should get 0xff  
}

TEST(Driver_Test, UsecaseV1_SyncWrite) { 
  uint8_t tx_data[] = {0xff, 0xff, 0x01, 0x04, 0x02, 0x2B, 0x01, 0xcc};
  MockHwDriver mock_hw{};
  DxlDriver driver = DxlDriver::create<DxlProtocolV1, 128, 128>(mock_hw);
  
  driver.setTxIns(0x01, DxlProtocolV1::Ins::kSyncWrite);
  driver.writeTxByte(0x2b);
  driver.writeTxByte(0x01);
  driver.beginTransmission();
  EXPECT_EQ(mock_hw.mTxBufSize, sizeof(tx_data));
  EXPECT_TRUE(bufferIsEq(mock_hw.pTxBuf, tx_data, sizeof(tx_data)));
  EXPECT_EQ(driver.poll(), DxlDriver::kTransmitting);
  
  mock_hw.mTxDone = true;
  EXPECT_EQ(driver.poll(), DxlDriver::kReceiving);

  // when timeout is reached
  mock_hw.mTimeout = true;  
  EXPECT_EQ(driver.poll(), DxlDriver::kDone);
  EXPECT_EQ(driver.readRxByte(), 0xff);  // no data, should get 0xff
  EXPECT_EQ(driver.readRxWord(), 0xffff);  // no data, should get 0xffff  
}

TEST(Driver_Test, UsecaseV1_Read) {  
  uint8_t tx_data[] = {0xff, 0xff, 0x01, 0x04, 0x02, 0x2B, 0x01, 0xcc};
  uint8_t rx_data[] = {0xff, 0xff, 0x01, 0x03, 0x00, 0x20, 0xdb};
  MockHwDriver mock_hw{};
  DxlDriver driver = DxlDriver::create<DxlProtocolV1, 128, 128>(mock_hw);
  
  driver.setTxIns(0x01, DxlProtocolV1::Ins::kRead);
  driver.writeTxByte(0x2b);
  driver.writeTxByte(0x01);
  driver.beginTransmission();
  EXPECT_EQ(mock_hw.mTxBufSize, sizeof(tx_data));
  EXPECT_TRUE(bufferIsEq(mock_hw.pTxBuf, tx_data, sizeof(tx_data)));
  EXPECT_EQ(driver.poll(), DxlDriver::kTransmitting);  
  mock_hw.mTxDone = true;
  EXPECT_EQ(driver.poll(), DxlDriver::kReceiving);

  mock_hw.mockReceive(rx_data, sizeof(rx_data) - 1);
  EXPECT_EQ(driver.poll(), DxlDriver::kReceiving);

  mock_hw.mockReceive(rx_data[sizeof(rx_data) - 1]);
  EXPECT_EQ(driver.poll(), DxlDriver::kDone);

  EXPECT_EQ(driver.getRxId(), 0x01);
  EXPECT_EQ(driver.getRxStatusByte(), 0);  
  EXPECT_EQ(driver.readRxWord(), 0xffff);  // not enough data, should get 0xff  
  EXPECT_EQ(driver.readRxByte(), 0x20);
  EXPECT_EQ(driver.readRxByte(), 0xff);  // no more data, should get 0xff  
}

}  // namespace