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

import java.text.DecimalFormat;

import umich.framjack.ApplicationInterface.UpdateListener;

import android.os.Bundle;
import android.app.Activity;
import android.graphics.Color;
import android.view.Menu;

import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.Switch;
import android.widget.TextView;

public class FramJack extends Activity {
	private ApplicationInterface _appInterface;
	
	private TextView _tvIn1;
	private TextView _tvIn2;
	private TextView _tvIn3;
	private TextView _tvIn4;
	
	private TextView _tvTemp;
	private TextView _tvStatus;
	
	private Switch _swOut1;
	private Switch _swOut2;
	private Switch _swOut3;
	private Switch _swOut4;
	
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fram_jack);
        _appInterface = new ApplicationInterface();
        
        bindControls();
        
        _appInterface.registerOnUpdateListener(_listener);
        attachCheckChangeListeners();
    }
    
    private UpdateListener _listener = new UpdateListener() {
		public void Update() {
			runOnUiThread(new Runnable() {
				public void run() {
					updateBindings();
				}
			});
		}
    };

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_fram_jack, menu);
        return true;
    }
    
    @Override
    public void onPause() {
    	_appInterface.stop();
    	super.onPause();
    }
    
    @Override
    public void onResume() {
    	_appInterface.start();
    	super.onResume();
    }
    
    private void bindControls() {
    	_tvIn1 = (TextView) findViewById(R.id.textViewIn1);
    	_tvIn2 = (TextView) findViewById(R.id.textViewIn2);
    	_tvIn3 = (TextView) findViewById(R.id.textViewIn3);
    	_tvIn4 = (TextView) findViewById(R.id.textViewIn4);
    	
    	_tvStatus = (TextView) findViewById(R.id.textViewStatus);
    	_tvTemp = (TextView) findViewById(R.id.textViewTemp);
    	
    	_swOut1 = (Switch) findViewById(R.id.switch1);
    	_swOut2 = (Switch) findViewById(R.id.switch2);
    	_swOut3 = (Switch) findViewById(R.id.switch3);
    	_swOut4 = (Switch) findViewById(R.id.switch4);
    }
    
    private void updateBindings() {
    	_tvIn1.setText(_appInterface.getDigitalInput(0) ? R.string.high : R.string.low);
    	_tvIn2.setText(_appInterface.getDigitalInput(1) ? R.string.high : R.string.low);
    	
    	DecimalFormat decimalFormat = new DecimalFormat("#.##");
    	
    	_tvIn3.setText(decimalFormat.format(_appInterface.getAnalogInput(2)));
    	_tvIn4.setText(decimalFormat.format(_appInterface.getAnalogInput(3)));
    	
    	_tvStatus.setText(_appInterface.getIsConnected() ? R.string.status_connected : R.string.status_disconnected);
    	_tvStatus.setTextColor(_appInterface.getIsConnected() ? Color.parseColor("#11FF11") : Color.parseColor("#FF1111"));
    	
    	_tvTemp.setText(decimalFormat.format(_appInterface.getTemperature()));
    }
    
    private void attachCheckChangeListeners() {
    	_swOut1.setOnCheckedChangeListener(generateListener(0));
    	_swOut2.setOnCheckedChangeListener(generateListener(1));
    	_swOut3.setOnCheckedChangeListener(generateListener(2));
    	_swOut4.setOnCheckedChangeListener(generateListener(3));
    }
    
    private OnCheckedChangeListener generateListener(final int outputId) {
    	return new OnCheckedChangeListener() {
			public void onCheckedChanged(CompoundButton buttonView,
					boolean isChecked) {
				_appInterface.setOutput(outputId, isChecked);
			}
    	};
    }
    
    
}
