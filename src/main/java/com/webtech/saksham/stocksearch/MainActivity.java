package com.webtech.saksham.stocksearch;

import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.Filter;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;

import com.nhaarman.listviewanimations.itemmanipulation.DynamicListView;
import com.nhaarman.listviewanimations.itemmanipulation.swipedismiss.OnDismissCallback;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements AdapterView.OnItemClickListener {

    public static final String PREFS = "quotejsonprefs";
    public static final String FAV = "favprefs";
    public static final String COLORTEXT = "colorprefs";

    //String userString1;

    private Button mClear, mQuote;
    private TextView mTextView;
    String userString1, companysymbolString, swipedString,textString="", companysymbolStatus;
    List<String> responseList = new ArrayList<String>();
    private AutoCompleteTextView mAutoComplete;
    private ListView mListView;
    private DynamicListView mDynamicListView;
    String[] finalStringLL, splitString;
    ArrayList<Bean_Fav> beanresults;
    MyCustomListAdapter ma;
    static String searchText;
    TextView colorText;
    ImageButton mRefresh;
    public double f_takechange;


    String flName, flPrice, flChange, flCap, flSymbol, fltakeChange;
    ArrayList<Bean_Fav> fav_results = new ArrayList<Bean_Fav>();
    URL js;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //setup buttons and input field
        mClear = (Button) findViewById(R.id.clear_btn);
        mQuote = (Button) findViewById(R.id.quote_btn);
        mTextView = (TextView) findViewById(R.id.textView_test);
        mAutoComplete = (AutoCompleteTextView)findViewById(R.id.auto_names);
        //mListView = (ListView) findViewById(R.id.favlist_xml);
        mDynamicListView = (DynamicListView) findViewById(R.id.dynamiclistview);
        colorText =(TextView)findViewById(R.id.change_layout);
        mRefresh = (ImageButton)findViewById(R.id.refreshbtn);

        SuggestionAdapter suggestionAdapter = new SuggestionAdapter(this, R.layout.autocomplete_view, mAutoComplete.getText().toString());
        mAutoComplete.setAdapter(suggestionAdapter);
        mAutoComplete.setOnItemClickListener(this);




       // Gson gson = new Gson();
        SharedPreferences showfav = getSharedPreferences(FAV, 0);
        userString1 = showfav.getString("favcompany", "");
        splitString = userString1.split("//");
        for(int i =0; i<splitString.length;i++) {

            new FavTask().execute("http://stocksearch-1276.appspot.com/stocksearch.php?stockselect=" + splitString[i]);
        }
        //finalStringLL = new ArrayList<String>(Arrays.asList(split));
        //final String[] finalString = userString1.split("//");
        //showfav.edit().clear().commit();
        /*for(int i=0;i<finalString.length;i++) {
            mTextView.append(finalString[i]);
        }*/


        /*
        final ArrayAdapter<String> favListAdapter = new ArrayAdapter<String>(
                this,android.R.layout.simple_list_item_1,finalStringLL
        );*/
        //mListView.setAdapter(favListAdapter);

           // ArrayList<Bean_Fav> beanresults = getbeanresults();
            //mDynamicListView.setAdapter(new MyCustomListAdapter(this, beanresults));

        mDynamicListView.enableSwipeToDismiss(
                new OnDismissCallback() {
                    @Override
                    public void onDismiss(@NonNull final ViewGroup listView, @NonNull final int[] reverseSortedPositions) {
                        for (int position : reverseSortedPositions) {
                            fav_results.remove(position);
                            ma.notifyDataSetChanged();
                            System.out.println(position+splitString[position]);
                            swipedString = userString1.replace(splitString[position]+"//","");

                            SharedPreferences savefav = getSharedPreferences(FAV, 0);
                            SharedPreferences.Editor editor1 = savefav.edit();
                            editor1.putString("favcompany", swipedString);
                            editor1.apply();

                        }
                    }
                }
        );


        mDynamicListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position,
                                    long id) {
                new QuoteTask().execute("http://stocksearch-1276.appspot.com/stocksearch.php?stockselect=" + splitString[position]);
                searchText = splitString[position];

            }
        });






        //setup listeners
        mQuote.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                textString = mAutoComplete.getText().toString();
                if (textString.equals("")) {
                    AlertDialog.Builder AD = new AlertDialog.Builder(MainActivity.this);
                    AD.setMessage("Please enter a stock name or symbol");
                    AD.setPositiveButton("OK", new DialogInterface.OnClickListener(){
                        @Override
                            public void onClick(DialogInterface arg0, int arg1){

                    }
                    });
                    AlertDialog AD1 = AD.create();
                    AD1.show();
                } else {
                    searchText = mAutoComplete.getText().toString();

                        new QuoteTask().execute("http://stocksearch-1276.appspot.com/stocksearch.php?stockselect=" + searchText);

                }
            }
        });

        mClear.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            //Clear functionality
                mAutoComplete.setText(null);
            }
        });

       /* mRefresh.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                //Clear functionality
                mDynamicListView.setAdapter(null);
                ma.notifyDataSetChanged();
                for(int i =0; i<splitString.length;i++) {

                    new FavTask().execute("http://stocksearch-1276.appspot.com/stocksearch.php?stockselect=" + splitString[i]);
                }
            }
        }); */

    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            String selected = mAutoComplete.getText().toString();

    }


    //PRIVATE ARRAY LIST WENT HERE



    public class JSONTask extends AsyncTask<String, String, String>{

        @Override
        protected String doInBackground(String... params){
            HttpURLConnection connection = null;
            BufferedReader reader = null;
            try{
                URL url = new URL(params[0]);
                connection = (HttpURLConnection) url.openConnection();
                connection.connect();

                InputStream stream = connection.getInputStream();

                reader = new BufferedReader(new InputStreamReader(stream));

                StringBuffer buffer = new StringBuffer();

                String line = "";
                while ((line = reader.readLine()) != null){
                    buffer.append(line);
                }

                return buffer.toString();

            } catch(MalformedURLException e){
                e.printStackTrace();
            } catch(IOException e){
                e.printStackTrace();
            } finally{
                if(connection != null) {
                    connection.disconnect();
                }
                try {
                    if(reader != null) {
                        reader.close();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return null;
        }

        @Override
        protected void onPostExecute(String result){
            super.onPostExecute(result);
             try {
                JSONArray json = new JSONArray(result);

                List<String> responseList = new ArrayList<String>();
                for(int i = 0; i < json.length(); i++){
                    final JSONObject e = json.getJSONObject(i);
                    String name = e.getString("value");
                    String description = e.getString("label");
                    responseList.add(name);
                    responseList.add(description);
                    System.out.println("HI"+responseList);
                }
                 ArrayAdapter<String> adapter = new ArrayAdapter<String>(MainActivity.this, android.R.layout.simple_dropdown_item_1line, responseList);
                 mAutoComplete.setThreshold(1);
                 mAutoComplete.setAdapter(adapter);
            } catch (JSONException e) {
                 e.printStackTrace();
             }
        }
    }

    public class QuoteTask extends AsyncTask<String, String, String>{

        @Override
        protected String doInBackground(String... params){
            HttpURLConnection connection = null;
            BufferedReader reader = null;
            try{
                URL url = new URL(params[0]);
                connection = (HttpURLConnection) url.openConnection();
                connection.connect();

                InputStream stream = connection.getInputStream();

                reader = new BufferedReader(new InputStreamReader(stream));

                StringBuffer buffer = new StringBuffer();

                String line = "";
                while ((line = reader.readLine()) != null){
                    buffer.append(line);
                }

                return buffer.toString();

            } catch(MalformedURLException e){
                e.printStackTrace();
            } catch(IOException e){
                e.printStackTrace();
            } finally{
                if(connection != null) {
                    connection.disconnect();
                }
                try {
                    if(reader != null) {
                        reader.close();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return null;
        }

        @Override
        protected void onPostExecute(String result){
            super.onPostExecute(result);

            SharedPreferences jsonresult = getSharedPreferences(PREFS, 0);
            SharedPreferences.Editor editor = jsonresult.edit();
            try {
                JSONObject companysymbolObject = new JSONObject(result);
                companysymbolStatus = companysymbolObject.getString("Status");
                companysymbolString = companysymbolObject.getString("Symbol");


            } catch (JSONException e) {
                e.printStackTrace();
            }

            if(companysymbolStatus.equals("SUCCESS")) {


                editor.putString("symboljson", companysymbolString);
                editor.apply();
                editor.putString("quotejson", result);
                editor.apply();


                Intent i = new Intent(MainActivity.this, SecondScreen.class);
                startActivity(i);
            }else{
                AlertDialog.Builder AD = new AlertDialog.Builder(MainActivity.this);
                AD.setMessage("Please enter a stock name or symbol");
                AD.setPositiveButton("OK", new DialogInterface.OnClickListener(){
                    @Override
                    public void onClick(DialogInterface arg0, int arg1){

                    }
                });
                AlertDialog AD1 = AD.create();
                AD1.show();
            }

            //mTextView.setText(result);
        }
    }

    public class FavTask extends AsyncTask<String, String, String>{

        @Override
        protected String doInBackground(String... params){
            HttpURLConnection connection = null;
            BufferedReader reader = null;
            try{
                URL url = new URL(params[0]);
                connection = (HttpURLConnection) url.openConnection();
                connection.connect();

                InputStream stream = connection.getInputStream();

                reader = new BufferedReader(new InputStreamReader(stream));

                StringBuffer buffer = new StringBuffer();

                String line = "";
                while ((line = reader.readLine()) != null){
                    buffer.append(line);
                }

                return buffer.toString();

            } catch(MalformedURLException e){
                e.printStackTrace();
            } catch(IOException e){
                e.printStackTrace();
            } finally{
                if(connection != null) {
                    connection.disconnect();
                }
                try {
                    if(reader != null) {
                        reader.close();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return null;
        }

        @Override
        protected void onPostExecute(String result){
            super.onPostExecute(result);


            try {
                JSONObject companysymbolObject1 = new JSONObject(result);
                flName = companysymbolObject1.getString("Name");
                flPrice = companysymbolObject1.getString("LastPrice");
                flChange = companysymbolObject1.getString("ChangePercent");
                flCap = companysymbolObject1.getString("MarketCap");
                flSymbol = companysymbolObject1.getString("Symbol");
                fltakeChange = companysymbolObject1.getString("Change");

                SharedPreferences colorset = getSharedPreferences(COLORTEXT, 0);
                SharedPreferences.Editor coloreditor = colorset.edit();
                coloreditor.putString("colorjson", fltakeChange);
                coloreditor.apply();



                ArrayList<Bean_Fav> beanresults = getbeanresults();
                ma = new MyCustomListAdapter(getBaseContext(), beanresults);
                mDynamicListView.setAdapter(ma);



            } catch (JSONException e) {
                e.printStackTrace();
            }
        }


    }

    public class SuggestionAdapter extends ArrayAdapter<SuggestGetSet>{
        private List<SuggestGetSet> suggestions;
        private int viewResouceId;

        public SuggestionAdapter(Activity context, int viewResouceId, String nameFilter){
            super(context, viewResouceId);
            suggestions = new ArrayList<SuggestGetSet>();
            this.viewResouceId = viewResouceId;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent){
            View v = convertView;

            if(v==null){
                LayoutInflater vi = (LayoutInflater)getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                v = vi.inflate(viewResouceId, null);
            }
            SuggestGetSet suggestion = getItem(position);
            if(suggestion != null){
                TextView stockOptionSymbol = (TextView) v.findViewById(R.id.stockSymbol);
                TextView stockOptionName = (TextView) v.findViewById(R.id.stockName);
                if(stockOptionName!=null && stockOptionSymbol!=null){
                    stockOptionName.setText(suggestion.getName());
                    stockOptionSymbol.setText(suggestion.getId());
                }
            }
            return v;
        }
        @Override
        public int getCount() {
            return suggestions.size();
        }

        @Override
        public SuggestGetSet getItem(int index) {
            return suggestions.get(index);
        }

        @Override
        public Filter getFilter() {
            Filter myFilter = new Filter() {
                @Override
                protected FilterResults performFiltering(CharSequence constraint) {
                    FilterResults filterResults = new FilterResults();
                    JsonParse jp=new JsonParse();
                    if (constraint != null) {

                        List<SuggestGetSet> new_suggestions = jp.getParseJsonWCF(constraint.toString());
                        suggestions.clear();
                        for (int i=0;i<new_suggestions.size();i++) {
                            suggestions.add(new_suggestions.get(i));
                        }
                        filterResults.values = suggestions;
                        filterResults.count = suggestions.size();
                    }
                    return filterResults;
                }

                @Override
                protected void publishResults(CharSequence constraint, Filter.FilterResults results) {
                    if (results != null && results.count > 0) {
                        notifyDataSetChanged();
                    } else {
                        notifyDataSetInvalidated();
                    }
                }
            };
            return myFilter;
        }
    }


    private ArrayList<Bean_Fav> getbeanresults(){


                Bean_Fav bFav = new Bean_Fav();
                bFav.setSymbol(flSymbol);
                bFav.setName(flName);
        double f_changepercent = Double.valueOf(flChange);
        String f_cp = String.format("%.2f", f_changepercent);
                bFav.setChange(f_cp+" %");
                bFav.setPrice("$ "+flPrice);

        f_takechange = Double.valueOf(fltakeChange);



        if(f_takechange > 0){
            bFav.setColor(Color.GREEN);
        }else if(f_takechange <0 ){
            bFav.setColor(Color.RED);
        }

        double marketcap = Double.valueOf(flCap);
        if (marketcap > 9999999) {
            marketcap = marketcap / 1000000000;
            String marketcapdisplay = String.format("%.2f", marketcap);
            bFav.setCap(marketcapdisplay + " Billion");
        } else {
            marketcap = marketcap / 1000000;
            String marketcapdisplay = String.format("%.2f", marketcap);
            bFav.setCap(marketcapdisplay + " Million");
        }

                System.out.println("Outside: " + flName);
                fav_results.add(bFav);
                return fav_results;
    }

    public class JsonParse {
        private static final String TAG = "debug1";
        private static final String TAG1 = "debug2";


        public JsonParse(){}


        public List<SuggestGetSet> getParseJsonWCF(String sName)
        {
            List<SuggestGetSet> ListData = new ArrayList<SuggestGetSet>();
//        JSONParser parser = new JSONParser();
            try {
                String temp=sName.replace(" ", "%20");

                    js = new URL("http://stocksearch-1276.appspot.com/stocksearch.php?droidterm=" + temp);

                URLConnection jc = js.openConnection();
                BufferedReader reader = new BufferedReader(new InputStreamReader(jc.getInputStream()));
                String line = reader.readLine();

//            JSONObject jsonResponse = new JSONObject(line);
//            JSONArray jsonArray = jsonResponse.getJSONArray("results");
                JSONArray jsonArray = new JSONArray(line);

                for(int i = 0; i < jsonArray.length(); i++){
                    JSONObject r = jsonArray.getJSONObject(i);
                    ListData.add(new SuggestGetSet(r.getString("id"),r.getString("label")));
                }
            } catch (Exception e1) {
                // TODO Auto-generated catch block
                e1.printStackTrace();
            }
            return ListData;
        }

    }
}

