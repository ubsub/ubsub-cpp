#include <inttypes.h>

#ifndef ubsub_h
#define ubsub_h

// Create a payload to a buf, returning total number of bytes
// Will write header, signature, and payload to buf
int createPacket(uint8_t* buf, int bufSize, const char* topic, const char* key, const uint8_t* payload, int payloadSize);

// Validates payload and returns pointer within datagram to the message
// If validation fails, returns NULL
const uint8_t* getValidatedPayload(const uint8_t* datagram, int datagramSize, const char* key);

// Send a packet with topic, key, and message as null-terminated string
int sendEvent(const char* topic, const char* key, const char* message);

// Send packet with uint8_t* payload rather than string
int sendEvent(const char* topic, const char* key, const uint8_t* payload, int payloadSize);

// Send packet with custom buffer
int sendEvent(uint8_t* buf, int bufSize, const char* topic, const char* key, const uint8_t* payload, int payloadSize);

//TODO: Implement wrappers using String for particle/arduino

#endif
