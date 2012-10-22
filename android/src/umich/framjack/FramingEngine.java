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

import java.util.ArrayList;

public class FramingEngine {
	private IncomingPacketListener _incomingListener;
	private OutgoingByteListener _outgoingListener;
	
	private final int _receiveMax = 18;
	
	private enum ReceiveState { START, SIZE, DATA, DATA_ESCAPE};
	private ReceiveState _receiveState = ReceiveState.START;
	private ArrayList<Integer> _receiveBuffer;
	private int _receiveSize;
	
	private ArrayList<Integer> _transmitBuffer;
	
	public FramingEngine() {
		_receiveBuffer = new ArrayList<Integer>();
		_transmitBuffer = new ArrayList<Integer>();
	}
	
	public void receiveByte(int val) {
		//System.out.print(val + " ");
		if (val == 0xDD && _receiveState != ReceiveState.DATA_ESCAPE) {
			_receiveState = ReceiveState.SIZE;
			_receiveBuffer.clear();
			return;
		}
		
		switch (_receiveState) {
			case DATA:
				if (val == 0xCC) {
					_receiveState = ReceiveState.DATA_ESCAPE;
					_receiveSize--;
					break;
				}
				_receiveBuffer.add(val);
				if (_receiveBuffer.size() == _receiveSize) {
					checkPacket();
					_receiveState = ReceiveState.START;
				}
				break;
			case DATA_ESCAPE:
				_receiveState = ReceiveState.DATA;
				_receiveBuffer.add(val);
				if (_receiveBuffer.size() == _receiveSize) {
					checkPacket();
					_receiveState = ReceiveState.START;
				}
				break;
			case SIZE:
				if (val > _receiveMax) {
					_receiveState = ReceiveState.START;
					break;
				}
				_receiveSize = val;
				_receiveState = ReceiveState.DATA;
				break;
			default:
				break;
		}
	}
	
	private void checkPacket() {
		int sum = 0;
		for (int i = 0; i < _receiveBuffer.size() - 1; i++) {
			//System.out.print(Integer.toHexString(_receiveBuffer.get(i)) + " ");
			sum += _receiveBuffer.get(i);
		}
		//System.out.println(Integer.toHexString(_receiveBuffer.get(_receiveBuffer.size() - 1)));
		
		if ((sum & 0xFF) == _receiveBuffer.get(_receiveBuffer.size() - 1)) {
			int[] retArray = new int[_receiveBuffer.size() - 1];
			for (int i = 0; i < _receiveBuffer.size() - 1; i++) {
				retArray[i] = _receiveBuffer.get(i);
			}
			_incomingListener.IncomingPacketReceive(retArray);
		}		
	}
	
	public void transmitByte(int val) {
		if (val == 0xCC) {
			_transmitBuffer.add(0xDD);
		}
		_transmitBuffer.add(val);
	}
	
	public void transmitEnd() {
		int[] toSend = new int[_transmitBuffer.size() + 3];
		toSend[0] = 0xDD;
		toSend[1] = _transmitBuffer.size() + 1;
		for (int i = 0; i < _transmitBuffer.size(); i++) {
			toSend[i + 2] = _transmitBuffer.get(i);
			toSend[_transmitBuffer.size() + 2] += toSend[i + 2];
		}
		
		toSend[_transmitBuffer.size() + 2] &= 0xFF;
		_transmitBuffer.clear();
		_outgoingListener.OutgoingByteTransmit(toSend);
	}
	
	public void registerIncomingPacketListener(IncomingPacketListener listener) {
		_incomingListener = listener;
	}
	
	public void registerOutgoingByteListener(OutgoingByteListener listener) {
		_outgoingListener = listener;
	}
	
	public interface IncomingPacketListener {
		public abstract void IncomingPacketReceive(int[] packet);
	}
	
	public interface OutgoingByteListener {
		public abstract void OutgoingByteTransmit(int[] outgoingRaw); 
	}
}
