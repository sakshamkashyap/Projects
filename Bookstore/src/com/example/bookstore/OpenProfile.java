package com.example.bookstore;

import java.util.ArrayList;
import java.util.HashMap;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.AlertDialog;
import android.app.ListActivity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
//import android.widget.SimpleAdapter;

public class OpenProfile extends ListActivity implements OnClickListener{

	private ProgressDialog pDialog;
	AlertDialog alertDialog;
	//SimpleAdapter adapter;
	private Button b5;
	
	
	
	private static final String READ_PROFILE_URL ="http://crshaggy.byethost7.com/webservice/profile.php";
	
	private static final String TAG_USERNAME = "username";
	private static final String TAG_POSTS = "posts";
	private static final String TAG_BOOK = "book";
	private static final String TAG_ISSUE = "issue";
	private static final String TAG_RETURNED = "returned";

	private JSONArray mComments = null;

	
	private ArrayList<HashMap<String, String>> mCommentList;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.open_profile);
		b5=(Button)findViewById(R.id.database);
		b5.setOnClickListener(this);
		
		
		
}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
	    // Inflate the menu items for use in the action bar
	    MenuInflater inflater = getMenuInflater();
	    inflater.inflate(R.menu.main_activity_actions, menu);
	    return super.onCreateOptionsMenu(menu);
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
	    // Handle presses on the action bar items
	        if(item.getItemId()== R.id.action_about){
	            openSettings();
	            
	    }
			return true;
	}
	
		@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		// loading the comments via AsyncTask
		new LoadComments().execute();
	}
	
		public void addComment(View v) {
			Intent i = new Intent(OpenProfile.this, UpdateList.class);
			startActivity(i);
		}
		
	public void updateJSONdata() {
	
	mCommentList = new ArrayList<HashMap<String, String>>();
	JSONParser jParser = new JSONParser();
	JSONObject json = jParser.getJSONFromUrl(READ_PROFILE_URL);
	
	try{
		mComments = json.getJSONArray(TAG_POSTS);
		
		for (int i = 0; i < mComments.length(); i++) {
			JSONObject c = mComments.getJSONObject(i);
			
			String username = c.getString(TAG_USERNAME);
			String book = c.getString(TAG_BOOK);
			String issue = c.getString(TAG_ISSUE);
			String returned = c.getString(TAG_RETURNED);
			
		HashMap<String, String> map = new HashMap<String, String>();
		 
		
		map.put(TAG_USERNAME, username);
		map.put(TAG_BOOK, book);
		map.put(TAG_ISSUE, issue);		
		map.put(TAG_RETURNED, returned);
		
		mCommentList.add(map);
	}
} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	
	 void updateList() {
	
		ListAdapter adapter = new SimpleAdapter(this, mCommentList, 
		R.layout.random, new String[] { TAG_USERNAME,TAG_BOOK, TAG_ISSUE,TAG_RETURNED}, 
		new int[] { R.id.username, R.id.book, R.id.issue_date, R.id.return_date});
		
		setListAdapter(adapter);
		
	

		 		ListView lv = getListView();	
		lv.setOnItemClickListener(new OnItemClickListener() {
		
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				
				
					}
				});
			}
			
	public class LoadComments extends AsyncTask<Void, Void, Boolean>{
	
		@Override
		protected void onPreExecute() {
			super.onPreExecute();
			pDialog = new ProgressDialog(OpenProfile.this);
			pDialog.setMessage("Loading updated profile...");
			pDialog.setIndeterminate(false);
			pDialog.setCancelable(true);
			pDialog.show();
		}

		@Override
		protected Boolean doInBackground(Void... arg0) {
			updateJSONdata();
			return null;

		}

		@Override
		protected void onPostExecute(Boolean result) {
			super.onPostExecute(result);
			pDialog.dismiss();
			updateList();
			
				
}

	}

	@Override
	public void onClick(View v) {
		Intent j=new Intent(OpenProfile.this, DBLib.class);
		startActivity(j);
		
	}
	
	public void openSettings(){
		alertDialog = new AlertDialog.Builder(OpenProfile.this).create();
		alertDialog.setTitle("About Developer");
		alertDialog.setMessage("Developed By: Saksham Kashyap\n" +
				"Project Mentor: Dr. V.Senthil\n" +
				"Defence Research and Development Organisation");
		alertDialog.show();
		}
}


				
