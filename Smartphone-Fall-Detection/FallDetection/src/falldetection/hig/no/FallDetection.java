package falldetection.hig.no;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.os.PowerManager;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

/**
 * 4-5: Should the onStop/Pause/Destroy methods stop the service or allow it to continue running in the background?
 */

/**
 * This is an activity class that shows the GUI-interface of the application.
 * Users can choose the sensitivity level, telephone number and an optional
 * e-mail address for which a fall alarm will be sent to. This is also where the
 * user starts and stops the application for fall monitoring.
 */
public class FallDetection extends Activity implements OnItemClickListener {
	private static final String TAG = "FallDetection.Main";
	private static final boolean debug = false;
	private static Button start_service1;
	private static Button log_msg_btn;
	private static Button clear_msg_btn;
	private static boolean showingMessages=false;
	private static boolean savingData=false;
	private TextView log_messages;
	private ListView ADLListView;
	
	private String ADLClassChosen = "";
	
	/**
	 * This variable is true when the SensorService is started, false otherwise.
	 */
	private static Boolean service1Started;
	/**
	 * These two variables keep the phone from fully going to sleep. Note: We
	 * might want to experiment with removing this completely if not needed or
	 * moving it to the SensorService.
	 */
	private PowerManager pm;
	private PowerManager.WakeLock w1;
	
	//This variable is used to save data to the phone.
	private String str1="";
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		if (debug)
			Log.i(TAG, "oncreate() started");

		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		log_messages = (TextView) findViewById(R.id.log_messages);
		start_service1 = (Button) findViewById(R.id.start_service1);
		log_msg_btn = (Button) findViewById(R.id.log_msg_btn);
		clear_msg_btn= (Button) findViewById(R.id.clear_msg_btn);
		TextView tv = (TextView) findViewById(R.id.main_tv);

		log_messages.setText("");
		log_msg_btn.setText("Show messages");
		if(!savingData){
			clear_msg_btn.setText("Append data to files");
		}
		else{
			clear_msg_btn.setText("Stop appending data");
		}
		
		pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		w1 = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);

		tv.append(TAG + "-> oncreate() started");
		
		if (isMyServiceRunning("SensorService")){
			service1Started = true;
			start_service1.setText("Stop Service");
		}
		else {
			service1Started = false;
			start_service1.setText("Start Service");
		}
			
		ADLListView = ( ListView ) findViewById( R.id.ADLSelectionListView );
		
		ADLListView.setOnItemClickListener( this );
	}

	@Override
	protected void onResume() {
		if (debug)
			Log.i(TAG, "onResume()");

		super.onResume();
	}

	@Override
	protected void onRestart() {
		if (debug)
			Log.i(TAG, "onRestart()");

		super.onRestart();
	}

	@Override
	protected void onPause() {
		if (debug)
			Log.i(TAG, "onPause()");

		super.onPause();
	}

	@Override
	protected void onStop() {
		if (debug)
			Log.i(TAG, "onStop()");

		super.onStop();
	}

	@Override
	public void onBackPressed() {
		if (debug)
			Log.i(TAG, "onBackPressed()");

		super.onBackPressed();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (debug)
			Log.i(TAG, "onActivityResult() started -> RequestCode: "
					+ requestCode + "\tResultCode: " + resultCode);
	}

	@Override
	protected void onDestroy() {
		if (debug)
			Log.i(TAG, "onDestroy()");

		super.onDestroy();
		this.finish();
	}

	public void exit_btn_click(View v) {
		if (debug)
			Log.i(TAG, "exit_btn_click()");

		this.finish();
	}

	/**
	 * This is called when the start button is clicked. When the button is
	 * clicked a screen wake lock and a service are either created or stopped
	 * depending on if a service is already running.
	 * 
	 * @param v
	 */
	public void start_service1_click(View v) {
		if (service1Started) {
			start_service1.setText("Start Service");
			service1Started = false;
			if (w1.isHeld()){
					w1.release();
			}
			
			if (stopService(new Intent(FallDetection.this, SensorService.class))){
				if (debug)
					Log.i(TAG, "service stopped successfully.");
			} 
			else {
				if (debug)
					Log.i(TAG, "service did not stop!");
			}
		} 
		else {
			if (debug)
				Log.i(TAG, "service started");

			start_service1.setText("Stop Service");
			service1Started = true;
			if (!w1.isHeld())
				w1.acquire();

			//AWFUL WAY TO SOLVE THIS PROBLEM. DON'T CARE THOUGH, D.
			Global.ADLClassChosen = ADLClassChosen;
			
			Intent intent = new Intent( FallDetection.this, SensorService.class );

			startService( intent );
		
		}
	}

	/**
	 * This is called when the start button is clicked. When the button is
	 * clicked a screen wake lock and a service are either created or stopped
	 * depending on if a service is already running.
	 * 
	 * @param v
	 */
	
				
	public void log_msg_btn_click(View v){
		if (showingMessages) {
			log_msg_btn.setText("Show Messages");
			showingMessages = false;
			log_messages.setText("");
			
		}
		else {
			log_msg_btn.setText("Close Messages");
			showingMessages = true;
			
				
			log_messages.setMovementMethod(new ScrollingMovementMethod());
			
			log_messages.append((CharSequence)(SensorService.getMsg())+"\n");
			
		}
		 
	}
	
	public void clear_msg_btn_click(View v){
		if (!savingData) {
			clear_msg_btn.setText("Stop appending data");
			SensorService.startTesting();
			savingData = true;
			
		}
		
		else{			
			clear_msg_btn.setText("Append data to files");
			
			SensorService.stopTesting();
			SensorService.clearMsg();
			savingData=false;
		
		}
			
	}
	
	
	private boolean isMyServiceRunning(String className) {
		String name = "falldetection.hig.no." + className;
		ActivityManager manager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
		for (RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
			if (name.equals(service.service.getClassName())) {
				return true;
			}
		}
		return false;
	}
/*
	public void start_btn_click_old(View v) {
		if (serviceStarted) {
			start_btn.setText("Start Monitoring");
			new Thread(new Runnable() {
				public void run() {
					serviceStarted = false;
					if (w1.isHeld())
						w1.release();

					if (stopService(new Intent(FallDetection.this,
							SensorService.class))) {
						if (debug)
							Log.i(TAG, "service stopped successfully.");
					}

					else {
						if (debug)
							Log.i(TAG, "service did not stop!");
					}

				}
			}).start();
		} 
		else {
			if (debug)
				Log.i(TAG, "service started");

			start_btn.setText("Stop Monitoring");
			new Thread(new Runnable() {
				public void run() {
					serviceStarted = true;
					if (!w1.isHeld())
						w1.acquire();

					startService(new Intent(FallDetection.this, SensorService.class));
					startService(new Intent(FallDetection.this,	SensorService2.class));
					startService(new Intent(FallDetection.this, SensorService3.class));
				}
			}).start();
		}
	}*/

	@Override
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
		// TODO Auto-generated method stub
		
		ADLClassChosen = (String) ( ( TextView ) arg1 ).getText( );
	}
}