#include "mock.h"
#include <stdexcept>

size_t MockHwDriver::available() {
  size_t x = 0;
  if (rx_read_index_ < rx_fill_index_) {
    x = rx_fill_index_ - rx_read_index_;
  }
  return x;
}

bool MockHwDriver::txIsDone() {
  return mTxDone;
}

bool MockHwDriver::isTimeout() {
  return mTimeout;
}

uint8_t MockHwDriver::read() {
  if (rx_read_index_ >= rx_fill_index_) {
    throw std::out_of_range("rx buffer is empty");
  }
  if (rx_read_index_ >= mRxBufSize) {
    throw std::out_of_range("tried to read outside buffer");
  }
  return pRxBuf[rx_read_index_++];
}

void MockHwDriver::beginTransmission(uint8_t* tx_buffer, size_t tx_buffer_size, uint8_t* rx_buffer, size_t rx_buffer_size, size_t expected_reply_size) {
  pTxBuf = tx_buffer;
  pRxBuf = rx_buffer;
  mTxBufSize = tx_buffer_size;
  mRxBufSize = rx_buffer_size;
  mExpectedReplySize = expected_reply_size;
}

void MockHwDriver::mockInit() {
  rx_fill_index_ = 0;
  rx_read_index_ = 0;
}

void MockHwDriver::mockReceive(uint8_t data) {
  if (rx_fill_index_ >= mRxBufSize) {
    throw std::out_of_range("Rx buffer overflow");
  }
  pRxBuf[rx_fill_index_++] = data;
}

void MockHwDriver::mockReceive(uint8_t* buf, size_t len) {
  for (size_t i=0; i<len; i++) {
    if (rx_fill_index_ >= mRxBufSize) {
      throw std::out_of_range("Rx buffer overflow");
    }
    pRxBuf[rx_fill_index_++] = buf[i];
  }
}
