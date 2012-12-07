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

package umich.framjack;

import java.util.Timer;
import java.util.TimerTask;

import umich.framjack.core.FramingEngine;
import umich.framjack.core.OnByteSentListener;
import umich.framjack.core.OnBytesAvailableListener;
import umich.framjack.core.SerialDecoder;
import umich.framjack.core.FramingEngine.IncomingPacketListener;
import umich.framjack.core.FramingEngine.OutgoingByteListener;

public class ApplicationInterface {
	private SerialDecoder _serialDecoder;
	private FramingEngine _framer;
	
	private final double _scalingVoltage = 1.8;
	
	private boolean[] _digitalOutputState = new boolean[] {false, false, false, false};
	private boolean[] _digitalInputState = new boolean[] {false, false};
	private double[] _analogInputState = new double[] {0.0, 0.0, 0.0};
	
	private boolean _isConnected = false;
	private boolean _hasUpdated = false;
	
	private int _pendingTransmitBytes = 0;
	
	private Timer _watchdogConnectionTimer;
	private UpdateListener _listener = null;
	
	public ApplicationInterface() {
		_framer = new FramingEngine();
		_serialDecoder = new SerialDecoder();
		
		
		_serialDecoder.registerBytesAvailableListener(_bytesAvailableListener);
		_serialDecoder.registerByteSentListener(_byteSentListener);
		_framer.registerIncomingPacketListener(_incomingPacketListener);
		_framer.registerOutgoingByteListener(_outgoingByteListener);	
		
		int[] toSend = encode();
		for (int i = 0; i < toSend.length; i++) {
			_framer.transmitByte(toSend[i]);
		}
		_framer.transmitEnd();
		
	}
	
	///////////////////////////////////////////////
	// Listeners
	///////////////////////////////////////////////	
	private OutgoingByteListener _outgoingByteListener = new OutgoingByteListener() {
		public void OutgoingByteTransmit(int[] outgoingRaw) {
			synchronized (ApplicationInterface.this) {
				_pendingTransmitBytes += outgoingRaw.length;
			}
			
			for (int i = 0; i < outgoingRaw.length; i++) {
				_serialDecoder.sendByte(outgoingRaw[i]);
			}
		}
	};
	
	private IncomingPacketListener _incomingPacketListener = new IncomingPacketListener() {
		public void IncomingPacketReceive(int[] packet) {
			for (int i = 0; i < packet.length; i++) {
				System.out.print(Integer.toHexString(packet[i]) + " ");
			}
			System.out.println();
			decodeAndUpdate(packet);
		}
	};
	
	private OnByteSentListener _byteSentListener = new OnByteSentListener() {
		public void onByteSent() {
			synchronized (ApplicationInterface.this) {
				_pendingTransmitBytes--;
				if (_pendingTransmitBytes == 0) {
					int[] toSend = encode();
					for (int i = 0; i < toSend.length; i++) {
						_framer.transmitByte(toSend[i]);
					}
					_framer.transmitEnd();
				}
			}	
		}
	};
	
	private OnBytesAvailableListener _bytesAvailableListener = new OnBytesAvailableListener() {
		public void onBytesAvailable(int count) {
			while(count > 0) {
				int byteVal = _serialDecoder.readByte();
				//System.out.println("Received: " + byteVal);
				_framer.receiveByte(byteVal);
				count--;
			}
		}
	};
	
	///////////////////////////////////////////////
	// Building the Byte Stream
	///////////////////////////////////////////////	
	private void decodeAndUpdate(int[] packet) {
		if (packet.length != 9) {
			return;
		}
		
		synchronized (this) {
			
			_hasUpdated = true;
			
			for (int i = 0; i < 2; i++) {
				_digitalInputState[i] = ((packet[0] >> i) & 0x01) == 1;
			}
			
			double[] rawAdcValues = new double[4];
			
			rawAdcValues[1] = ((packet[1 + 1*2] & 0xFF) << 8) | (packet[2 + 1*2] & 0xFF);
			rawAdcValues[2] = (((packet[1 + 2*2] & 0xFF) << 8) | (packet[2 + 2*2] & 0xFF)) >> 1;
			rawAdcValues[3] = (((packet[1 + 3*2] & 0xFF) << 8) | (packet[2 + 3*2] & 0xFF)) >> 1;
				
			_analogInputState[0] = (double)rawAdcValues[1] / 1000;
			_analogInputState[1] = (double)rawAdcValues[2] / 1000;
			_analogInputState[2] = (double)rawAdcValues[3] / 1000;
		}
		
		OnUpdateListener();
	}
	
	private int[] encode() {
		int retValue = 0;
		for (int i = 0; i < 4; i++) {
			if (_digitalOutputState[i]) {
				retValue |= (1 << i);
			}
		}
		
		return new int[] {retValue};
	}
	
	///////////////////////////////////////////////
	// Start/Stop Stuff =)
	///////////////////////////////////////////////	
	
	public void start() {
		_serialDecoder.start();
		_watchdogConnectionTimer = new Timer();
		_watchdogConnectionTimer.scheduleAtFixedRate(new TimerTask() {
			@Override
			public void run() {
				synchronized (ApplicationInterface.this) {
					_isConnected = _hasUpdated;
					_hasUpdated = false;
				}
				OnUpdateListener();
			}
		}, 1000, 1000);
	}
	
	public void stop() {
		_watchdogConnectionTimer.cancel();
		_watchdogConnectionTimer = null;
		_serialDecoder.stop();
		
		_isConnected = false;
		OnUpdateListener();
	}

	///////////////////////////////////////////////
	// Public Interface
	///////////////////////////////////////////////	
	
	public synchronized void setOutput(int outputId, boolean isHigh) {
		_digitalOutputState[outputId] = isHigh;
	}
	
	public synchronized void registerOnUpdateListener(UpdateListener listener) {
		_listener = listener;
	}
	
	public synchronized boolean getDigitalInput(int inputId) {
		return _digitalInputState[inputId];
	}
	
	public synchronized double getAnalogInput(int inputId) {
		return _analogInputState[inputId - 1];
	}
	
	public synchronized double getTemperature() {
		return _analogInputState[0];
	}
	
	public synchronized boolean getIsConnected() {
		return _isConnected;
	}
	
	public interface UpdateListener {
		public abstract void Update();
	}
	
	///////////////////////////////////////////////
	// Helpers
	///////////////////////////////////////////////
	
	// NOT THREADSAFE
	public void OnUpdateListener() {
		if (_listener != null) {
			_listener.Update();
		}
	}
}
