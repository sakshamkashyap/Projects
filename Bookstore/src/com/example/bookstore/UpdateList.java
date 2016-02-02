package com.example.bookstore;

import java.util.ArrayList;
import java.util.List;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class UpdateList extends Activity implements OnClickListener{
	
	private EditText book, returned, issue;
	private Button  mSubmit;


	
	
	 // Progress Dialog
    private ProgressDialog pDialog;
 
    // JSON parser class
    JSONParser jsonParser = new JSONParser();
    
   
    private static final String POST_COMMENT_URL = "http://crshaggy.byethost7.com/webservice/updateprofile.php";
    
 
    //ids
    private static final String TAG_SUCCESS = "success";
    private static final String TAG_MESSAGE = "message";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.update_list);
		
		book = (EditText)findViewById(R.id.book);
		issue = (EditText)findViewById(R.id.issue);
		returned = (EditText)findViewById(R.id.returned);
		
		mSubmit = (Button)findViewById(R.id.button_up);
		mSubmit.setOnClickListener(this);
		
		
		
	}

	@Override
	public void onClick(View v) {
				new PostComment().execute();
	}
	
	
	class PostComment extends AsyncTask<String, String, String> {
		
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            pDialog = new ProgressDialog(UpdateList.this);
            pDialog.setMessage("Posting Comment...");
            pDialog.setIndeterminate(false);
            pDialog.setCancelable(true);
            pDialog.show();
        }
		
		@Override
		protected String doInBackground(String... args) {
			// TODO Auto-generated method stub
			 // Check for success tag
            int success;
            String post_book = book.getText().toString();
            String post_issue = issue.getText().toString();
            String post_returned = returned.getText().toString();
        	//String post_username=sp.getString("username", "Cannot load data");

            
        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(UpdateList.this);
           String post_username = sp.getString("username", "anon");
          //String post_username = "temp";
            try {
            	//String post_username=sp.getString("username", "Cannot load data");
            	
            	
                // Building Parameters
                List<NameValuePair> params = new ArrayList<NameValuePair>();
                params.add(new BasicNameValuePair("username", post_username));
                params.add(new BasicNameValuePair("book", post_book));
                params.add(new BasicNameValuePair("issue", post_issue));
                params.add(new BasicNameValuePair("returned", post_returned));
 
                Log.d("request!", "starting");
                
                //Posting user data to script 
                JSONObject json = jsonParser.makeHttpRequest(
                		POST_COMMENT_URL, "POST", params);
 
                // full json response
                Log.d("Post Comment attempt", json.toString());
 
                // json success element
                success = json.getInt(TAG_SUCCESS);
                if (success == 1) {
                	Log.d("Comment Added!", json.toString());    
                	finish();
                	return json.getString(TAG_MESSAGE);
                }else{
                	Log.d("Comment Failure!", json.getString(TAG_MESSAGE));
                	return json.getString(TAG_MESSAGE);
                	
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }
 
            return null;
			
		}
		
        protected void onPostExecute(String file_url) {
            // dismiss the dialog once product deleted
            pDialog.dismiss();
            if (file_url != null){
            	Toast.makeText(UpdateList.this, file_url, Toast.LENGTH_LONG).show();
            }
 
        }
		
	}
		 

}
