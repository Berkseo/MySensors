/*
 ATSHA204 signing backend. The Atmel ATSHA204 offers true random number generation and
 HMAC-SHA256 authentication with a readout-protected key.
 
 Created by Patrick "Anticimex" Fallberg <patrick@fallberg.net>
*/
#ifndef MySigningAtsha204_h
#define MySigningAtsha204_h

#include "MyConfig.h"
#include "MySigning.h"
#include "utility/ATSHA204.h"
#include <stdint.h>

#define SIGNING_IDENTIFIER (1)

#ifdef MY_SECURE_NODE_WHITELISTING
typedef struct {
	uint8_t nodeId;
	uint8_t serial[SHA204_SERIAL_SZ];
} whitelist_entry_t;
#endif

// The ATSHA204 is capable of generating proper random numbers for nonce
// and can calculate HMAC-SHA256 signatures. This is enterprise-
// level of security and ought to implement the signing needs for anybody.
class MySigningAtsha204 : public MySigning
{ 
public:
	MySigningAtsha204(bool requestSignatures=true,
#ifdef MY_SECURE_NODE_WHITELISTING
		uint8_t nof_whitelist_entries=0, const whitelist_entry_t* the_whitelist=NULL,
#endif
		uint8_t atshaPin=MY_ATSHA204_PIN);
	bool getNonce(MyMessage &msg);
	bool checkTimer(void);
	bool putNonce(MyMessage &msg);
	bool signMsg(MyMessage &msg);
	bool verifyMsg(MyMessage &msg);
private:
	atsha204Class atsha204;
	unsigned long timestamp;
	bool verification_ongoing;
	uint8_t current_nonce[NONCE_NUMIN_SIZE_PASSTHROUGH+SHA204_SERIAL_SZ+1];
	uint8_t temp_message[SHA_MSG_SIZE];
	uint8_t rx_buffer[SHA204_RSP_SIZE_MAX];
	uint8_t tx_buffer[SHA204_CMD_SIZE_MAX];
	bool calculateSignature(MyMessage &msg);
	uint8_t* sha256(const uint8_t* data, size_t sz);
#ifdef MY_SECURE_NODE_WHITELISTING
	uint8_t whitlist_sz;
	const whitelist_entry_t* whitelist;
#endif
};

#endif
