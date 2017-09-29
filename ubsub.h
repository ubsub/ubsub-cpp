#include <inttypes.h>

#ifndef ubsub_h
#define ubsub_h

int createPacket(uint8_t* buf, int bufSize, const char* topic, const char* key, const uint8_t* payload, int payloadSize);

const uint8_t* getValidatedPayload(const uint8_t* datagram, int datagramSize, const char* key);



#endif
