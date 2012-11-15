/*
 *  This file is part of hijack-infinity.
 *
 *  hijack-infinity is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  hijack-infinity is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with hijack-infinity.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __FRAMINGENGINE_H__
#define __FRAMINGENGINE_H__

#include "config.h"

#include <inttypes.h>

#include "gpio.h"
#include "hardware.h"

////////////////////////////////////////
// Public Memebers:
////////////////////////////////////////

typedef void fe_callback(void);
typedef void fe_packetReceived(uint8_t * buf, uint8_t len);
typedef uint8_t fe_byteSender(uint8_t);

// Initialize the internal structures of
// the framing engine.
void fe_init(void);

// Called by the byte-receiving machinery to
// pass a received byte into the framing engine.
void fe_handleByteReceived(uint8_t byte);

// Called by the byte-sending machinery to 
// notify the framing engine a byte has been sent
// allowing it to queue another byte.
void fe_handleByteSent(void);

// Register a callback to be invoked when a full packet
// has been received by the framing engine and verified
// to have a valid checksum.
void fe_registerPacketReceivedCb(fe_packetReceived * cb);

// Register a callback to be invoked when a full packet
// has been sent to allow for the application layer to
// queue another packet for transmission.
void fe_registerPacketSentCb(fe_callback * cb);

// Register a function that allows the framing engine
// to send a byte to the lower layers of communication.
void fe_registerByteSender(fe_byteSender * sender);

// Fills the TxBuffer with raw unpacketified data
// to be sent by the framing engine. Should be called
// only when first started and from within a PacketSentCb.
void fe_writeTxBuffer(uint8_t * buf, uint8_t len);

// Called once to initialize the callback-driven
// sending loop.
void fe_startSending(void);

////////////////////////////////////////
// Private Memebers:
////////////////////////////////////////

#define FE_INBUFFERSIZE 26
#define FE_OUTBUFFERSIZE 20
#define FE_RECEIVEMAX 18

enum fe_receiveStateEnum {
	fe_receiveState_start,
	fe_receiveState_size,
	fe_receiveState_data,
	fe_receiveState_dataEscape
};

struct fe_state_struct {

	// Holds the received raw data and the outgoing
	// full packet data.
	uint8_t incomingBuffer[FE_INBUFFERSIZE];
	uint8_t outgoingBuffer[FE_OUTBUFFERSIZE];
	uint8_t incomingBufferPos;
	uint8_t outgoingBufferPos;
	uint8_t outgoingBufferSize;

	// Where we build the incoming and outgoing
	// packets from. 
	// Incoming packet payload is written here
	// and outgoing packet payload is written here.
	uint8_t incomingPktBuffer[FE_INBUFFERSIZE];
	uint8_t outgoingPktBuffer[FE_OUTBUFFERSIZE];
	uint8_t incomingPktSize;
	uint8_t outgoingPktSize;


	fe_packetReceived * packetReceivedCb;
	fe_callback * packetSentCb;
	fe_byteSender * byteSender;

	enum fe_receiveStateEnum rxState;

	// Stores the received size of the incoming
	// packet.
	uint8_t receiveSize;
} fe_state;

void fe_checkPacket(void);
void fe_buildTransmitBuffer(void);

#endif