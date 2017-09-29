#include <string.h>
#include "ubsub.h"
#include "sha256.h"

const static int HEADER_SIZE = 73;

#ifdef ARDUINO
#error "Arduino not supported yet"
#elif PARTICLE || PLATFORM_ID
#include "Particle.h"
static uint32_t getTime() {
	return now();
}
static uint8_t getNonce() {
	return random(256) * random(256) * random(256) * random(256);
}
#else //PC?
#include <ctime>
#include <stdlib.h>
static uint32_t getTime() {
	return std::time(NULL);
}
static uint32_t getNonce() {
	srand(getTime()); //TODO: This isn't great
	return (uint32_t)rand() + (uint32_t)rand();
}
#endif

int createPacket(uint8_t* buf, int bufSize, const char* topic, const char* key, const uint8_t* payload, int payloadSize) {
	if (bufSize < payloadSize + HEADER_SIZE)
		return -1;

	const int topicLength = strlen(topic);
	if (topicLength > 32)
		return -1;

	memset(buf, 0, bufSize);

	// Write header
	*(uint32_t*)(buf+33) = getTime(); //ts
	*(uint32_t*)(buf+37) = getNonce(); //nonce
	memcpy(buf+41, topic, topicLength);

	// Create signature
	Sha256.initHmac((uint8_t*)key, strlen(key));
	Sha256.write(buf+33, 40); // The above header we've written thus far
	Sha256.write(payload, payloadSize);
	uint8_t* digest = Sha256.resultHmac();

	// Write signature header
	*(buf+0) = 0x1; // Version
	memcpy(buf+1, digest, 32);

	// Copy in the payload
	memcpy(buf+HEADER_SIZE, payload, payloadSize);

	return payloadSize + HEADER_SIZE;
}

const uint8_t* getValidatedPayload(const uint8_t* datagram, int datagramSize, const char* key) {
	if (datagramSize < HEADER_SIZE)
		return NULL;

	if (datagram[0] != 0x1) // Version
		return NULL;

	// Extract ts and validate
	uint32_t ts = *(uint32_t*)(datagram+33);
	uint32_t now = getTime();

	if (abs(ts - now) > 10)
		return NULL; // Failed timestamp, too old

	// Create our own digest
	Sha256.initHmac((uint8_t*)key, strlen(key));
	Sha256.write(datagram + 33, datagramSize - 33);
	uint8_t* digest = Sha256.resultHmac();

	// Compare
	if (memcmp(datagram + 1, digest, 32) != 0)
		return NULL;

	return datagram + HEADER_SIZE;
}
