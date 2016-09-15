package com.webtech.saksham.stocksearch;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.Toast;

import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.login.LoginManager;
import com.facebook.share.Sharer;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.widget.ShareDialog;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


public class SecondScreen extends AppCompatActivity {

    public static final String PREFS = "quotejsonprefs";
    public static final String FAV = "favprefs";
    public static final String FAV2 = "favprefs";
    String[] FavListArray ;
    StringBuilder tempString = new StringBuilder();
    String finalObject, appendedString = "", deletedString = "";
    JSONArray favcompanyarray = new JSONArray();
    int x = 1;

    TabLayout mTabLayout;
    ViewPager mViewPger;
    private Menu menu;
    MenuItem mStar, mFB;
    String fname,fsymbol,fstockprice;

    private CallbackManager cmngr;
    private LoginManager login;


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu items for use in the action bar
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu_layout, menu);
        this.menu = menu;

        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle presses on the action bar items
        switch (item.getItemId()) {
            case R.id.fb_actionbtn:
                onfbClick();
                return true;

            case android.R.id.home:
                this.finish();
                Intent i = new Intent(SecondScreen.this, MainActivity.class);
                startActivity(i);
                return true;



            case R.id.star_btn:
                onStarClick();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.second_screen);
FacebookSdk.sdkInitialize(getApplicationContext());
        mViewPger = (ViewPager)findViewById(R.id.viewPager);
        mViewPger.setAdapter(new ViewPageAdapter(getSupportFragmentManager(),getApplicationContext()));

        //TableRow tv = (TableRow) findViewById(R.id.stocksymbol);
        //Button fav_btn = (Button) findViewById(R.id.fb_btn);
        mStar = (MenuItem) findViewById(R.id.fb_actionbtn);
        mFB = (MenuItem) findViewById(R.id.star_btn);

        ActionBar actionBar = getSupportActionBar();
        actionBar.setHomeButtonEnabled(true);
        actionBar.setDisplayHomeAsUpEnabled(true);

        mTabLayout = (TabLayout)findViewById(R.id.tabLayout);
        mTabLayout.setupWithViewPager(mViewPger);



        mTabLayout.setOnTabSelectedListener(new TabLayout.OnTabSelectedListener(){

            @Override
            public void onTabSelected(TabLayout.Tab tab) {
                mViewPger.setCurrentItem((tab.getPosition()));
            }

            @Override
            public void onTabUnselected(TabLayout.Tab tab) {
                mViewPger.setCurrentItem((tab.getPosition()));
            }

            @Override
            public void onTabReselected(TabLayout.Tab tab) {
                mViewPger.setCurrentItem((tab.getPosition()));
            }
        });

        SharedPreferences example = getSharedPreferences(PREFS, 0);
        String userString = example.getString("quotejson", "");


        try {
            JSONObject parentObject = new JSONObject(userString);
            finalObject = parentObject.getString("Symbol");
            fname = parentObject.getString("Name");
            fsymbol = parentObject.getString("Symbol");
            fstockprice = parentObject.getString("LastPrice");
            //tv.setText(finalObject);

            SharedPreferences showfav2 = getSharedPreferences(FAV2, 0);
            appendedString = showfav2.getString("favcompany", "");


        } catch (JSONException e) {
            e.printStackTrace();
        }


       if(appendedString.contains(MainActivity.searchText+"//")){
            //delete the name
            menu.getItem(0).setIcon(getResources().getDrawable(R.drawable.gold_fav_star));
        }else{
            //add the name
            menu.getItem(0).setIcon(getResources().getDrawable(R.drawable.white_fav_star));
        }
    }

    class ViewPageAdapter extends FragmentPagerAdapter {

        private String fragments[] = {"CURRENT","HISTORICAL","NEWS"};

        public ViewPageAdapter(FragmentManager supportFragmentManager, Context applicationContext){
            super(supportFragmentManager);
        }

        @Override
        public Fragment getItem(int position) {
            switch (position) {
                case 0:
                    return new QuoteFragment();

                case 1:
                    return new ChartsFragment();

                case 2:
                    return new NewsFragment();
                default:
                    return null;
            }
        }

        @Override
        public int getCount(){
            return fragments.length;
        }

        @Override
        public CharSequence getPageTitle(int position){
            return fragments[position];
        }
    }

    public void onStarClick() {

        //FavListArray = appendedString.split("//");
        if(appendedString.contains(finalObject)){
            //delete the name
            menu.getItem(0).setIcon(getResources().getDrawable(R.drawable.white_fav_star));
            x=0;
        }else{
            //add the name
            menu.getItem(0).setIcon(getResources().getDrawable(R.drawable.gold_fav_star));
            x=2;
        }

        if (x==0) {//deleting


            deletedString = appendedString.replace(finalObject+"//", "");

            System.out.println("Deleted String: " + deletedString);

            SharedPreferences savefav = getSharedPreferences(FAV, 0);
            SharedPreferences.Editor editor1 = savefav.edit();
            editor1.putString("favcompany", deletedString);
            editor1.apply();

            //menu.getItem(0).setIcon(getResources().getDrawable(R.drawable.white_fav_star));

        }else if(x==2) {//adding
            //Gson gson = new Gson();

            SharedPreferences savefav = getSharedPreferences(FAV, 0);
            SharedPreferences.Editor editor1 = savefav.edit();
            //jsonvalue = gson.toJson(finalObject);
            //favcompanyarray.add(jsonvalue.toString());
            // tempString.append(finalObject);

            appendedString += finalObject+"//";
            editor1.putString("favcompany", appendedString);
            editor1.apply();

            System.out.println("UPDATED STRING: "+appendedString);
            //menu.getItem(0).setIcon(getResources().getDrawable(R.drawable.gold_fav_star));

        }
    }

    public void onfbClick(){
        FacebookSdk.sdkInitialize(getApplicationContext());
        cmngr = CallbackManager.Factory.create();
        final ShareDialog shareDialog = new ShareDialog(this);
        // Button post = (Button) findViewById(R.id.button3);

        //  post.setOnClickListener(new View.OnClickListener() {
        //    public void onClick(View v) {
        if (ShareDialog.canShow(ShareLinkContent.class)) {
            String title="Current Stock Price of "+fname+","+fstockprice;
            String description="Stock Information of "+fname;
            String url="http://chart.finance.yahoo.com/t?s="+fsymbol+"&lang=en-US&width=400&height=400";
            String imageurl="http://chart.finance.yahoo.com/t?s="+fsymbol+"&lang=en-US&width=400&height=300";

            ShareLinkContent linkContent = new ShareLinkContent.Builder()
                    .setContentTitle(title)
                    .setContentDescription(description)
                    .setContentUrl(Uri.parse(url))
                    .setImageUrl(Uri.parse(imageurl))
                    .build();

            shareDialog.show(linkContent);
        }
        shareDialog.registerCallback(cmngr, new FacebookCallback<Sharer.Result>() {
            @Override
            public void onSuccess(Sharer.Result result) {
                Toast.makeText(getApplicationContext(), "Posted! shared successfully", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onCancel() {
                Toast.makeText(getApplicationContext(), "Post Cancelled", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onError(FacebookException error)
            {

            }
        });
        //              }
//            });

    }


}
