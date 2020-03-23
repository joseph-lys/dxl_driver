#include "DxlDriver.h"
#include "DxlProtocolV1.h"
#include "mock.h"
#include "gtest/gtest.h"
#include <iostream>
#include "utils.h"

namespace {
MockHwDriver mock_hw{};


TEST(Driver_Test, UsecaseV1_Ping) { 
  uint8_t tx_data[] = {0xff, 0xff, 0x01, 0x02, 0x01, 0xfb};
  uint8_t rx_data[] = {0xff, 0xff, 0x01, 0x02, 0x00, 0xfc};
  DxlDriver driver = DxlDriver::create<DxlProtocolV1, 128, 128>(mock_hw);
  driver.setTxIns(0x01, DxlProtocolV1::Ins::kPing);
  driver.beginTransmission();
  EXPECT_EQ(mock_hw.mTxBufSize, sizeof(tx_data));
  EXPECT_TRUE(bufferIsEq(mock_hw.pTxBuf, tx_data, sizeof(tx_data)));
  EXPECT_EQ(driver.poll(), DxlDriver::kTransmitting);
  
  mock_hw.mTxDone = true;
  mock_hw.mockInitReceive();
  EXPECT_EQ(driver.poll(), DxlDriver::kReceiving);
  
  mock_hw.mockReceive(rx_data, sizeof(rx_data));
  EXPECT_EQ(driver.poll(), DxlDriver::kDone);
  EXPECT_EQ(driver.getRxId(), 0x01);
  EXPECT_EQ(driver.getRxStatusByte(), 0);  
}

TEST(Driver_Test, UsecaseV1_Write) { 
  DxlDriver driver = DxlDriver::create<DxlProtocolV1, 128, 128>(mock_hw);

}
TEST(Driver_Test, UsecaseV1_SyncWrite) { 
  DxlDriver driver = DxlDriver::create<DxlProtocolV1, 128, 128>(mock_hw);

}
TEST(Driver_Test, UsecaseV1_Read) { 
  DxlDriver driver = DxlDriver::create<DxlProtocolV1, 128, 128>(mock_hw);

}

}  // namespace