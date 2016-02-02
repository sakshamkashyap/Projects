package com.example.bookstore;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public class StartingPoint extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.starting_point);
		
		Thread logo = new Thread(){
			public void run(){
				try{
					sleep(3000);
					Intent i = new Intent (StartingPoint.this, Login.class);
					startActivity(i);
				}catch(InterruptedException e){
					e.printStackTrace();
				}
				finally{
					finish();
				}
			}
		};
		logo.start(); //45668ec
	}

	
}
