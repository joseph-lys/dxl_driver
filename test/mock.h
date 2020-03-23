#ifndef MOCK_H_
#define MOCK_H_

#include "IfHwDxlDriverBase.h"

/// Mock IfHwDxlDriver implementation
class MockHwDriver : public IfHwDxlDriverBase {
 public:  // public for easy manipulation during testing
  bool mTxDone = false;
  bool mTimeout = false;
  uint8_t* pRxBuf = nullptr;
  uint8_t* pTxBuf = nullptr;
  size_t mRxBufSize = 0;
  size_t mTxBufSize = 0;
  size_t mExpectedReplySize = 0;
 private: 
  size_t rx_fill_index_ = 0;  // index of Rx buffer to write next byte
  size_t rx_read_index_ = 0;  // index of Rx buffer to read next byte
 public:
  size_t available() override;  
  bool txIsDone() override;
  bool isTimeout() override;
  uint8_t read() override;
  void beginTransmission(uint8_t* tx_buffer, size_t tx_buffer_size, uint8_t* rx_buffer, size_t rx_buffer_size, size_t expected_reply_size);

  /// Helper functions for RX buffer manipulation
  ///
  /// reset the receive buffer
  void mockInit(); 
  /// "receive" some bytes
  void mockReceive(uint8_t* buf, size_t len);
  /// "receive" a single byte
  void mockReceive(uint8_t data);
};





#endif  // MOCK_H_