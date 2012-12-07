package umich.framjack.apps.freqsweep;

import umich.framjack.core.FramingEngine;
import umich.framjack.core.OnByteSentListener;
import umich.framjack.core.OnBytesAvailableListener;
import umich.framjack.core.SerialDecoder;
import umich.framjack.core.FramingEngine.IncomingPacketListener;
import umich.framjack.core.FramingEngine.OutgoingByteListener;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {

	private SerialDecoder _serialDecoder;
	private FramingEngine _framer;
	
	private boolean _nextFlag = false;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
		_framer = new FramingEngine();
		_serialDecoder = new SerialDecoder();
		
		_serialDecoder.registerBytesAvailableListener(_bytesAvailableListener);
		_serialDecoder.registerByteSentListener(_byteSentListener);
		_framer.registerIncomingPacketListener(_incomingPacketListener);
		_framer.registerOutgoingByteListener(_outgoingByteListener);	
		
		final Button b1 = (Button)findViewById(R.id.button1);
		b1.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				EditText startFreqEditText = (EditText)findViewById(R.id.startFreq);
				EditText endFreqEditText = (EditText)findViewById(R.id.endFreq);
				EditText msStepEditText = (EditText)findViewById(R.id.stepMs);
				EditText hzStepEditText = (EditText)findViewById(R.id.stepHz);
			
				final int startFreq = Integer.parseInt(startFreqEditText.getText().toString());
				final int endFreq = Integer.parseInt(endFreqEditText.getText().toString());
				final int msStep = Integer.parseInt(msStepEditText.getText().toString());
				final int hzStep = Integer.parseInt(hzStepEditText.getText().toString());
				
				b1.setText("Running...");
				
				Thread t = new Thread(new Runnable() {
					@Override
					public void run() {
						int currentFreq = startFreq;
						
						while (currentFreq <= endFreq) {
							_serialDecoder.setFreq(currentFreq);
							currentFreq += hzStep;
							
							if (msStep == 0) {
								while (!_nextFlag) {
									try {
										Thread.sleep(100);
									} catch (InterruptedException e) {
										e.printStackTrace();
									}										
								}
								
								_nextFlag = false;
							}
							else {
								try {
									Thread.sleep(msStep);
								} catch (InterruptedException e) {
									e.printStackTrace();
								}									
							}
						}
						
						MainActivity.this.runOnUiThread(new Runnable() {
							@Override
							public void run() {
								b1.setText("Start");
							}
						});
					}
				});
				
				t.start();
			}
		});
		
		final Button b2 = (Button)findViewById(R.id.button2);
		b2.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				_nextFlag = true;
			}
		});
    }

    @Override
    public void onPause() {
    	_serialDecoder.stop();
    	super.onPause();
    }
    
    @Override
    public void onResume() {
    	_serialDecoder.start();
    	super.onResume();
    }
    
	///////////////////////////////////////////////
	// Listeners
	///////////////////////////////////////////////	
	private OutgoingByteListener _outgoingByteListener = new OutgoingByteListener() {
		public void OutgoingByteTransmit(int[] outgoingRaw) {
			synchronized (MainActivity.this) {
				//_pendingTransmitBytes += outgoingRaw.length;
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
			//decodeAndUpdate(packet);
		}
	};
	
	private OnByteSentListener _byteSentListener = new OnByteSentListener() {
		public void onByteSent() {
			synchronized (MainActivity.this) {
				//_pendingTransmitBytes--;
				//if (_pendingTransmitBytes == 0) {
				//	int[] toSend = encode();
				//	for (int i = 0; i < toSend.length; i++) {
				//		_framer.transmitByte(toSend[i]);
				//	}
				//	_framer.transmitEnd();
				//}
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
	
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }
}
