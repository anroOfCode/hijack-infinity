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

package umich.framjack.core;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;

/*
 * AudioInterface General Concepts
 * 
 * Goal of this interface: Hook up to the audio subsystem in android
 * and process the incoming data and sends output data.
 * 
 * Serial data in HiJack is manchester encoded (1 -> 10, 0 -> 01) and
 * frequency-shift key modulated. This means we have one frequency that
 * represents a 1, and twice that frequency represents a 0.
 * 
 * The purpose of the AudioInterface class is to process the raw data
 * and provide primitives to a manchester coding engine to process the
 * data and decode it into data usable by the host application. 
 * 
 * Inputs: Two defined interfaces:
 * 
 * OutgoingSource: Contains a function that returns true or false. Is
 * repeatedly invoked by the output engine to determine the next
 * frequency/bit.
 * 
 * IncomingSink: Is sent a length/value primitive with the length of the
 * last sustained measured frequency and if it was a high or low frequency.
 */

public class AudioReceiver {
	
	///////////////////////////////////////////////
	// Constants
	///////////////////////////////////////////////	
	
	// Most Android devices support 'CD-quality' sampling frequencies.
	final private int _sampleFrequency = 44100;
	
	// HiJack is powered at 21kHz
	private int _powerFrequency = 21000;
	
	// IO is FSK-modulated at either 613 or 1226 Hz (0 / 1)
	final private int _ioBaseFrequency = 613;

	///////////////////////////////////////////////
	// Main interfaces
	///////////////////////////////////////////////	
	
	// Used for receiving and transmitting the
	// primitive data elements (1s and 0s)
	private OutgoingSource _source = null;
	private IncomingSink _sink = null;
	
	// For Audio Output
    AudioTrack _audioTrack;
    Thread _outputThread;
    
    // For Audio Input
    AudioRecord _audioRecord;
    Thread _inputThread;
    
	///////////////////////////////////////////////
	// Output state
	///////////////////////////////////////////////		

	// For performance reasons we batch update
	// the audio output buffer with this many bits.
	final private int _bitsInBuffer = 100;

	// To ensure efficient computation we create some buffers. 
	private short[] _outHighHighBuffer;
	private short[] _outLowLowBuffer;
	private short[] _outHighLowBuffer;
	private short[] _outLowHighBuffer;
	
	private int _outBitBufferPos = 0;
	private int _powerFrequencyPos = 0;
	
	private short[] _stereoBuffer;
	private short[] _recBuffer;
	
	private boolean _isInitialized = false;
	private boolean _isRunning = false;
	private boolean _stop = false;
	
	///////////////////////////////////////////////
	// Input state
	///////////////////////////////////////////////	
	
	private enum SearchState { ZERO_CROSS, NEGATIVE_PEAK, POSITIVE_PEAK };
	private SearchState _searchState = SearchState.ZERO_CROSS;
	
	// Part of a circular buffer to find the peak of each
	// signal. 
	private int _toMean[] = new int[] {0, 0, 0};
	private int _toMeanPos = 0;
	
	// Part of a circular buffer to keep track of any
	// bias in the signal over the past 144 measurements
	private int _biasArray[] = new int[144];
	private boolean _biasArrayFull = false;
	private double _biasMean = 0.0;
	private int _biasArrayPos = 0;

	// Keeps track of the maximal value between two
	// zero-crossings to find the distance between
	// peaks
	private int _edgeDistance = 0;

	
	///////////////////////////////////////////////
	// Processors
	///////////////////////////////////////////////	
	
	private void updateOutputBuffer() {

		boolean isHigh[] = new boolean[_bitsInBuffer];
			
		for (int i = 0; i < _bitsInBuffer; i++) {
			isHigh[i] = _source.getNextBit();
		}
		
		int currentBit = -2;
		
		double powerMutiplier = Math.PI * (double)_powerFrequency / (double)_sampleFrequency * 2;
		
		for (int i = 0; i < _stereoBuffer.length/2; i++) {

			if (i % ((_outHighHighBuffer.length)) == 0) {
				currentBit += 2;
				_outBitBufferPos = 0;
			}
			
			// Choose if we're sending a 1 or 0.
			if (isHigh[currentBit] && isHigh[currentBit+1]) {
				_stereoBuffer[i*2] = _outHighHighBuffer[_outBitBufferPos++];
			}
			else if (!isHigh[currentBit] && !isHigh[currentBit+1]) {
				_stereoBuffer[i*2] = _outLowLowBuffer[_outBitBufferPos++];
			}
			else if (isHigh[currentBit] && !isHigh[currentBit+1]) {
				_stereoBuffer[i*2] = _outHighLowBuffer[_outBitBufferPos++];
			}
			else if (!isHigh[currentBit] && isHigh[currentBit+1]) {
				_stereoBuffer[i*2] = _outLowHighBuffer[_outBitBufferPos++];
			}
			
			// Toss the power signal on there. We keep a running signal across calls to this function
			// with the _powerFrequencyPos var to ensure the wave is continuous. 
			_stereoBuffer[i*2+1] =  (short) boundToShort(
				Math.sin(powerMutiplier * _powerFrequencyPos++) * 32786
			);	
		}
		
		// To prevent eventual overflows.
		_powerFrequencyPos = _powerFrequencyPos % (_sampleFrequency * _powerFrequency);
	}
	
	private void processInputBuffer(int shortsRead) {
		// We are basically trying to figure out where the edges are here,
		// in order to find the distance between them and pass that on to
		// the higher levels. 
		double meanVal = 0.0;
		
		for (int i = 0; i < shortsRead; i++) {
			
			meanVal = addAndReturnMean(_recBuffer[i]) - addAndReturnBias(_recBuffer[i]);
			_edgeDistance++;
			
			// Cold boot we simply set the search based on
			// where the first region is located. 
			if (_searchState == SearchState.ZERO_CROSS) {
				_searchState = meanVal < 0 ? SearchState.NEGATIVE_PEAK : SearchState.POSITIVE_PEAK;
			}
			
			// Have we just seen a zero transistion?
			if ((meanVal < 0 && _searchState == SearchState.POSITIVE_PEAK) ||
					(meanVal > 0 && _searchState == SearchState.NEGATIVE_PEAK)) {
				
				_sink.handleNextBit(_edgeDistance, _searchState == SearchState.POSITIVE_PEAK);
				_edgeDistance = 0;
				_searchState = (_searchState == SearchState.NEGATIVE_PEAK) ? SearchState.POSITIVE_PEAK : SearchState.NEGATIVE_PEAK;
			}
		}
	}
	
	///////////////////////////////////////////////
	// Incoming Bias and Smoothing Functions
	///////////////////////////////////////////////
	
	private double addAndReturnMean(int in) {
		_toMean[_toMeanPos++] = in;
		_toMeanPos = _toMeanPos % _toMean.length;
		
		double sum = 0.0;
		
		for (int i = 0; i < _toMean.length; i++) {
			sum += _toMean[i];
		}
		
		return sum / _toMean.length;
	}
	
	private double addAndReturnBias(int in) {
		if (_biasArrayFull) {
			_biasMean -= (double)_biasArray[_biasArrayPos] / (double)_biasArray.length;
		}

		_biasArray[_biasArrayPos++] = in;
		_biasMean += (double)in / (double)_biasArray.length;
		
		// If we're at the end of the bias array we move the
		// position back to 0 and recalculate the mean from scratch
		// keep small inaccuracies from influencing it. 
		if (_biasArrayPos == _biasArray.length) {
			double totalSum = 0.0;
			for (int i = 0; i < _biasArray.length; i++) {
				totalSum += _biasArray[i];
			}
			_biasMean = totalSum / (double)_biasArray.length;
			_biasArrayPos = 0;
			_biasArrayFull = true;
		}
		
		return _biasMean;
	}
	
	///////////////////////////////////////////////
	// Audio Interface
	// Note: these exist primarily to pass control to
	//       the responsible subfunctions. NO code here.
	///////////////////////////////////////////////	
	
    Runnable _outputGenerator = new Runnable() {       
        public void run() {
        	Thread.currentThread().setPriority(Thread.NORM_PRIORITY);
            
            while (!_stop) {
            	updateOutputBuffer();
                _audioTrack.write(_stereoBuffer, 0, _stereoBuffer.length);   
            }
        }
    };
    
    Runnable _inputProcessor = new Runnable() {
    	public void run() {
    		Thread.currentThread().setPriority(Thread.MIN_PRIORITY);
    		
    		while (!_stop) {
    			int shortsRead = _audioRecord.read(_recBuffer, 0, _recBuffer.length);
    			processInputBuffer(shortsRead);
    		}
    	}
    };
	
	///////////////////////////////////////////////
	// Public Interface 
	///////////////////////////////////////////////
	public int getPowerFrequency() {
		return _powerFrequency;
	}
	
	public void setPowerFrequency(int powerFrequency) {
		_powerFrequency = powerFrequency;
	}
	
	public void registerOutgoingSource(OutgoingSource source) {
		if (_isRunning) {
			throw new UnsupportedOperationException("AudioIO must be stopped to set a new source.");
		}
		_source = source;
		_source.getNextBit();
	}
	
	public void registerIncomingSink(IncomingSink sink) {
		if (_isRunning) {
			throw new UnsupportedOperationException("AudioIO must be stopped to set a new sink.");
		}		
		_sink = sink;
	}

	public void initialize() {
		// Create buffers to hold what a high and low 
		// frequency waveform looks like
		int bufferSize = getBufferSize();
		
		_outHighHighBuffer = new short[bufferSize];
		_outHighLowBuffer = new short[bufferSize];
		_outLowHighBuffer = new short[bufferSize];
		_outLowLowBuffer = new short[bufferSize];
		
		for (int i = 0; i < bufferSize; i++) {
			
			// NOTE: We bound this due to some weird java issues with casting to
			// shorts.+
			_outHighHighBuffer[i] = (short) (
				boundToShort(Math.sin((double)i * (double)2 * Math.PI * (double)_ioBaseFrequency / (double)_sampleFrequency) * Short.MAX_VALUE)
			);
			
			_outHighLowBuffer[i] = (short) (
				boundToShort(Math.sin((double)i * (double)4 * Math.PI * (double)_ioBaseFrequency / (double)_sampleFrequency) * Short.MAX_VALUE)
			); 

			_outLowLowBuffer[i] = (short) (
				boundToShort(Math.sin((double)(i + bufferSize) * (double)2 * Math.PI * (double)_ioBaseFrequency / (double)_sampleFrequency) * Short.MAX_VALUE)
			); 
			
			_outLowHighBuffer[i] = (short) (
				boundToShort(Math.sin((double)(i + bufferSize/2) * (double)4 * Math.PI * (double)_ioBaseFrequency / (double)_sampleFrequency) * Short.MAX_VALUE)
			); 
		}
		
		_isInitialized = true;
	}
	
	public void startAudioIO() {
		if (!_isInitialized) {
			initialize();
		}
		
		if (_isRunning) {
			return;
		}
		
		_stop = false;
		
		attachAudioResources();
		
		_audioRecord.startRecording();
		_audioTrack.play();
		
		_outputThread = new Thread(_outputGenerator);
		_inputThread = new Thread(_inputProcessor);
		
		_outputThread.start();
		_inputThread.start();
	}
	
	public void stopAudioIO() {
		_stop = true;	
		
		try {
			_outputThread.join();
			_inputThread.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		releaseAudioResources();
		
		_isRunning = false;
	}
	
	///////////////////////////////////////////////
	// Support functions
	///////////////////////////////////////////////
	
	private void attachAudioResources() {
		int bufferSize = getBufferSize();
		
		// The stereo buffer should be large enough to ensure
		// that scheduling doesn't mess it up.
		_stereoBuffer = new short[bufferSize * _bitsInBuffer];

		_audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, 
				_sampleFrequency, AudioFormat.CHANNEL_OUT_STEREO,
                AudioFormat.ENCODING_PCM_16BIT, 44100,
                AudioTrack.MODE_STREAM);     

		int recBufferSize = 
				AudioRecord.getMinBufferSize(_sampleFrequency, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
		_audioRecord = new AudioRecord(MediaRecorder.AudioSource.DEFAULT,
		        _sampleFrequency, AudioFormat.CHANNEL_IN_MONO,
		        AudioFormat.ENCODING_PCM_16BIT, recBufferSize);
		
		_recBuffer = new short[recBufferSize * 10];
	}
	
	private void releaseAudioResources() {
		_audioTrack.release();
		_audioRecord.release();
		
		_audioTrack = null;
		_audioRecord = null;		
		
		_stereoBuffer = null;
		_recBuffer = null;
	}
	
    private double boundToShort(double in) {
        return (in >= 32786.0) ? 32786.0 : (in <= -32786.0 ? -32786.0 : in );
    }
    
    private int getBufferSize() {
    	return _sampleFrequency / _ioBaseFrequency / 2;
    }
    
    
}


