#include "ubsub.h"
#include "sha256.h"

const static int HEADER_SIZE = 73;
const static uint16_t UBSUB_PORT = 4000;
const static char UBSUB_HOST[] = "router.ubsub.io";

// Grab the correct headers
#if PARTICLE
	#include <Particle.h>
#elif ARDUINO
	#include <Arduino.h>
	#include <ESP8266WiFi.h>
	#include <WiFiUdp.h>
#else
	#include <string.h>
	#include <ctime>
	#include <stdlib.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
#endif

static uint32_t getTime() {
#if ARDUINO
	return (uint32_t)(millis() / 1000);
#elif PARTICLE
	return now();
#else
	return std::time(NULL);
#endif
}

static uint8_t getNonce() {
#if ARDUINO || PARTICLE
	return (random(256) << 24) | (random(256) << 16) | (random(256) << 8) | random(256);
#else
	srand(getTime()); //TODO: This isn't great
	return (uint32_t)rand() + (uint32_t)rand();
#endif
}

static int sendDatagram(const uint8_t* buf, int bufSize) {
#if ARDUINO
#elif PARTICLE
#else
	static int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	struct hostent *server;
	server = gethostbyname(UBSUB_HOST);
	if (server == NULL)
		return -2;

	struct sockaddr_in serveraddr;
	bzero((char*)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(UBSUB_PORT);
	
	return sendto(sockfd, buf, bufSize, 0, (sockaddr*)&serveraddr, sizeof(serveraddr));
#endif
}

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

int sendEvent(uint8_t* buf, int bufSize, const char* topic, const char* key, const uint8_t* payload, int payloadSize) {
	int retSize = createPacket(buf, bufSize, topic, key, payload, payloadSize);
	if (retSize < 0)
		return retSize; // err

	return sendDatagram(buf, retSize);
}

int sendEvent(const char* topic, const char* key, const uint8_t* payload, int payloadSize) {
	const int bufSize = payloadSize + HEADER_SIZE;
	uint8_t* buf = (uint8_t*)malloc(bufSize);

	int ret = sendEvent(buf, bufSize, topic, key, payload, payloadSize);

	free(buf);
	return ret;
}

int sendEvent(const char* topic, const char* key, const char* message) {
	return sendEvent(topic, key, (uint8_t*)message, strlen(message));
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
