# UbSub-C

C++ Client and UDP verification for [UbSub.io](https://ubsub.io).

This client will allow for sending/receiving V1 UDP datagrams to trigger events on UbSub.

Currently, all you need is the sha256 and ubsub files.  In [ubsub.h](ubsub.h) you'll find all exposed functions
for various embedded platforms (including libc/unix compatibility).

# Usage

Usage can be as simple as sending an event. eg:
```c
#include "ubsub.h"
int main() {
	sendEvent("topicId", "topicKey", "hi there");
	return 0;
}
```

## Compatability

 * Unix/Linux

Future:

 * Arduino
 * Particle
 * ESP8266 Boards

# Third Party

## CryptoSuite

Sha256 implementation is from [Cathedrow/Cryptosuite](https://github.com/Cathedrow/Cryptosuite) with small modifications
for compatibility.

# License

Copyright (c) 2017 Christopher LaPointe

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
