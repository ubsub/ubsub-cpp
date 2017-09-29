#include "ubsub.h"
#include <iostream>
#include <string.h>

int main() {
	char topic[] = "B1xBAJNPib";
	char key[] = "HyH0yVvjb";
	char payload[] = "howdy there";

	uint8_t buf[4096];
	int sz = createPacket(buf, sizeof(buf), topic, key, (uint8_t*)payload, strlen(payload));

	if (getValidatedPayload(buf, sz, key) == NULL)
		std::cout << "INVALID";

	for (int i=0; i<sz; ++i) {
		std::cout << buf[i];
	}
}
