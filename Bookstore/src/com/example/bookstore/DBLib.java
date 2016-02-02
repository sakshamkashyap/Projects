package com.example.bookstore;

import java.util.ArrayList;
import java.util.HashMap;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.ListActivity;
import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class DBLib extends ListActivity {

	private ProgressDialog pDialog;
	private EditText inputSearch;
	SimpleAdapter adapter;
	
	
	private static final String READ_DB_URL ="http://crshaggy.byethost7.com/webservice/warehouse.php";
	
	private static final String TAG_TITLE = "title";
	private static final String TAG_POSTS = "posts";
	//private static final String TAG_PUBLISHER = "publisher";
	//private static final String TAG_AUTHOR1 = "author1";
	private static final String TAG_KEYWORD1 = "keyword1";

	private JSONArray mComments = null;

	
	private ArrayList<HashMap<String, String>> mCommentList;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.dblib_list);
		
		//inputSearch = (EditText)findViewById(R.id.et_search);
		//setListAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1));
}
		@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		// loading the comments via AsyncTask
		new LoadComments().execute();
	}
	
	public void updateJSONdata() {
	
	mCommentList = new ArrayList<HashMap<String, String>>();
	JSONParser jParser = new JSONParser();
	JSONObject json = jParser.getJSONFromUrl(READ_DB_URL);
	
	try{
		mComments = json.getJSONArray(TAG_POSTS);
		
		for (int i = 0; i < mComments.length(); i++) {
			JSONObject c = mComments.getJSONObject(i);
			
			String title = c.getString(TAG_TITLE);
			//String publisher = c.getString(TAG_PUBLISHER);
			//String author1 = c.getString(TAG_AUTHOR1);
			String keyword1 = c.getString(TAG_KEYWORD1);
			
		HashMap<String, String> map = new HashMap<String, String>();
		 
		
		map.put(TAG_TITLE, title);
		
		map.put(TAG_KEYWORD1, keyword1);
		
		mCommentList.add(map);
	}
} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	
	 void updateList() {
	
		adapter = new SimpleAdapter(this, mCommentList, 
		R.layout.post, new String[] { TAG_TITLE,TAG_KEYWORD1}, 
		new int[] { R.id.title, R.id.keyword1});
		
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
			pDialog = new ProgressDialog(DBLib.this);
			pDialog.setMessage("Loading DSL Books...");
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
			
			inputSearch = (EditText) findViewById(R.id.et_search);

	        inputSearch.addTextChangedListener(new TextWatcher() {

	            @Override
	            public void onTextChanged(CharSequence cs, int arg1, int arg2, int arg3) {
	                // When user changed the Text
	                DBLib.this.adapter.getFilter().filter(cs);   
	            }

	            @Override
	            public void beforeTextChanged(CharSequence arg0, int arg1, int arg2,
	                    int arg3) {
	                // TODO Auto-generated method stub

	            }

	            @Override
	            public void afterTextChanged(Editable arg0) {
	                // TODO Auto-generated method stub                          
	            }
	        });
			
				
}

	}
}


				
