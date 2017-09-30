#include "ubsub.h"
#include <iostream>
#include <string.h>

int main() {
	char topic[] = "S1eWDa7kr-";
	char key[] = "H1-v67kHb";
	char payload[] = "howdy there";

	// How to sign a packet yourself
	uint8_t buf[4096];
	int sz = createPacket(buf, sizeof(buf), topic, key, (uint8_t*)payload, strlen(payload));

	if (getValidatedPayload(buf, sz, key) == NULL)
		std::cout << "INVALID";

	for (int i=0; i<sz; ++i) {
		std::cout << buf[i];
	}

	// Sending a packet to ubsub
	sendEvent(topic, key, payload);
}
