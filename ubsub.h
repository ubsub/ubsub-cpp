#include <inttypes.h>

#ifndef ubsub_h
#define ubsub_h

uint8_t[32] generateHash(uint32_t ts, uint32_t nonce, uint8_t* buf, int bufLen);

#endif
