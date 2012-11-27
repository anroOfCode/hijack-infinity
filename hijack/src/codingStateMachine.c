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

#include "codingStateMachine.h"

/////////////////////////////////
// Region: Receive state machine
/////////////////////////////////

void csm_receiveIdle(void) {
    // If we see a 2T high period, it's a start bit!
    if (csm_state.curRx.signal == 0 &&
        csm_isWithinThreshold(csm_state.curRx.elapsedTime,
                                csm_state.lastRx.elapsedTime * 2)) {

        // we'll use the first time as a reference for
        // framing the rest of this byte.
        csm_state.deltaT = csm_state.lastRx.elapsedTime;

        csm_state.rxState = csm_receiveState_data;

        // We include the start bit in the rx bits
        // to make the algorithm a little simpler.
        csm_state.rxByte  = 0;
        csm_state.rxBits = 1;
    }
}

void csm_receiveData(void) {
    // Possible ways to receive data:
    // Two short pulses => Same bit as last bit
    // One long pulse => different bit
    // Anything else => nope nope nope.
    if (csm_isWithinThreshold(csm_state.curRx.elapsedTime, 
                                csm_state.deltaT)) {
        // The next bit is the same as the previous bit,
        // but we must wait for the next short pulse.
        csm_state.rxState = csm_receiveState_dataNext;
    }
    else if (csm_isWithinThreshold(csm_state.curRx.elapsedTime,
                                csm_state.deltaT * 2)) {
        // The next bit is different than the previous bit.
        // Set the bit and advance the bit position counter.
        if(((csm_state.rxByte >> (csm_state.rxBits - 1)) & 1) == 0) {
            csm_state.rxByte |= (1 << csm_state.rxBits);
        }
        
        csm_state.rxBits++;
        csm_advanceReceiveDataState();
    }
    else {
        // An error has occured, flip back to waiting
        // for a start bit.
        csm_state.rxState = csm_receiveState_idle;
    }
}

void csm_receiveDataNext(void) {
    // We're waiting for the second short pulse. The only time
    // we'll see two short pulses in a row is when the bit is
    // equal to the last bit. 
    if (csm_isWithinThreshold(csm_state.curRx.elapsedTime, 
                                csm_state.deltaT)) {
        // If the previous bit was a one, make this bit
        // a one also.
        if (((csm_state.rxByte >> (csm_state.rxBits - 1)) & 1) == 1) {
            csm_state.rxByte |= (1 << csm_state.rxBits);
        }

        csm_state.rxBits++;
        csm_advanceReceiveDataState();
    }
    else {
        // An error has occured, flip back to waiting
        // for a start bit.
        csm_state.rxState = csm_receiveState_idle;
    }
}

/////////////////////////////////
// Region: Transmit state machine
/////////////////////////////////
uint8_t csm_transmitIdle(void) {
    if (csm_state.txBitPosition && csm_state.txIdleCycles < IDLE_CYCLES) {
        csm_state.txIdleCycles++;
    }
    return csm_state.txBitPosition;
}

uint8_t csm_transmitPending(void) {
    if (csm_state.txBitPosition == 0) {
        csm_state.txState = csm_transmitState_data;
        return csm_transmitData();
    } else {
        return 1;
    }
}

uint8_t csm_transmitData(void) {
    uint8_t ret = ((csm_state.txByte >> csm_state.txBytePosition) & 0x1);


    if (csm_state.txBitPosition == 1) {
        csm_state.txBytePosition++;

        if (csm_state.txBytePosition == 10) {
            csm_state.txState = csm_transmitState_idle;
            csm_state.txBytePosition = 0;
            csm_state.txIdleCycles = 0;
        }
    }
    else {
         ret = !ret;       
    }

    return ret;
}

/////////////////////////////
// Region: Public functions
/////////////////////////////

void csm_receiveTiming(struct csm_timer_struct * in) {
    csm_state.curRx = (*in);
    csm_state.rxDispatch[csm_state.rxState]();
    csm_state.lastRx = (*in);
}

void csm_registerReceiveByte(csm_byteReceiver * func) {
    csm_state.rxCallback = func;
}

void csm_registerTransmitByte(csm_byteSent * func) {
    csm_state.txCallback = func;
}

uint8_t csm_advanceTransmitState(void) {
    return csm_state.txDispatch[csm_state.txState]();
}

void csm_finishAdvanceTransmitState(void) {
    csm_state.txBitPosition = csm_state.txBitPosition == 0 ? 1 : 0;

    if (csm_state.txIdleCycles == IDLE_CYCLES) {
        csm_state.txCallback();
        csm_state.txIdleCycles++;
    }
}


uint8_t csm_sendByte(uint8_t out) {
    if (csm_state.txState == csm_transmitState_idle &&
        csm_state.txIdleCycles > IDLE_CYCLES - 1) {
        csm_state.txState = csm_transmitState_pending;

        // Parity + (LSB) data + startbit
        uint8_t parity = csm_calcByteParity(out);
        csm_state.txByte = ((((uint16_t) out) << 1) |
                            (((uint16_t) parity) << 9));
        return 0;
    }
    else {
        return 1;
    }
}

void csm_init(void) {
    csm_state.txState = csm_transmitState_idle;

    csm_state.rxState = csm_receiveState_idle;
    csm_state.rxCallback = 0;
    csm_state.rxDispatch[0] = &csm_receiveIdle;
    csm_state.rxDispatch[1] = &csm_receiveData;
    csm_state.rxDispatch[2] = &csm_receiveDataNext;

    csm_state.txDispatch[0] = &csm_transmitIdle;
    csm_state.txDispatch[1] = &csm_transmitPending;
    csm_state.txDispatch[2] = &csm_transmitData;
    csm_state.txCallback = 0;
    csm_state.txIdleCycles = 0;
        
    csm_state.lastRx.elapsedTime = 0;
    csm_state.lastRx.signal = 0;

    // See config.h for declaration of these
    // platform specific parameters.
    csm_state.threshold = THRESHOLD;
    csm_state.deltaT = DELTAT;

    csm_state.txBitPosition = 0;
    csm_state.txBytePosition = 0;
}

////////////////////////////
// Region: Helper functions
////////////////////////////

uint8_t csm_isWithinThreshold(uint16_t value, uint16_t desired) {
    return value < desired + csm_state.threshold && 
        value > desired - csm_state.threshold ? 1 : 0;
}

uint8_t csm_calcByteParity(uint8_t byte) {
        // calculate parity function
        uint8_t parity = 0;
        uint8_t i = 0;
        while (i < 8) {
            parity ^= ((byte >> i) & 1);
            i++;
        }
        return parity;
}

void csm_advanceReceiveDataState(void) {
    if (csm_state.rxBits == 10) {
        // we have 1 start bit + 8 data bits + 1 parity bit

        if (csm_calcByteParity(csm_state.rxByte >> 1) == (csm_state.rxByte >> 9)) {
            csm_state.rxCallback((csm_state.rxByte >> 1) & 0xFF);
        }      

        csm_state.rxState = csm_receiveState_idle;
    }
    else {
        csm_state.rxState = csm_receiveState_data;
    }
}
