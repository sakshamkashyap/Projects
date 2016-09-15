package com.webtech.saksham.stocksearch;

import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;

import org.joda.time.DateTime;
import org.joda.time.format.DateTimeFormat;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.InputStream;

import uk.co.senab.photoview.PhotoViewAttacher;


public class QuoteFragment extends Fragment{


    public static final String PREFS = "quotejsonprefs";
    public static final String FAV = "favprefs";
    public static final String FAV2 = "favprefs";
    String[] FavListArray ;
    StringBuilder tempString = new StringBuilder();
    String appendedString = "", deletedString = "";
    JSONArray favcompanyarray = new JSONArray();
    int x = 1;
    PhotoViewAttacher mAttacher;
    ImageButton imageViewYahoo;

    String tName,tSymbol,tPrice,tChange,tTime,tCap,tChangeYTD,tHigh,tLow,tOpen;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {


        View rootView = inflater.inflate(R.layout.quotefrag, container, false);
        imageViewYahoo = (ImageButton) rootView.findViewById(R.id.yahooImg);


       // rYahoo.setOnClickListener(this);



        SharedPreferences example = this.getActivity().getSharedPreferences(PREFS, 0);
        String userString = example.getString("quotejson", "");

        try {

            JSONObject jsonObject = new JSONObject(userString);
            tSymbol = jsonObject.getString("Symbol");
            TextView test = (TextView) rootView.findViewById(R.id.stockAttribute1);
            test.setText("NAME");
            test = (TextView) rootView.findViewById(R.id.stockValue1);
            test.setText(jsonObject.getString("Name"));
            ImageView iw = (ImageView) rootView.findViewById(R.id.stockTrendImage1);
            iw.setImageResource(R.drawable.trans);

            test = (TextView) rootView.findViewById(R.id.stockAttribute2);
            test.setText("SYMBOL");
            test = (TextView) rootView.findViewById(R.id.stockValue2);
            test.setText(jsonObject.getString("Symbol"));
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage2);
            iw.setImageResource(R.drawable.trans);

            test = (TextView) rootView.findViewById(R.id.stockAttribute3);
            test.setText("LASTPRICE");
            test = (TextView) rootView.findViewById(R.id.stockValue3);
            test.setText("$ "+jsonObject.getString("LastPrice"));
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage3);
            iw.setImageResource(R.drawable.trans);

            test = (TextView) rootView.findViewById(R.id.stockAttribute4);
            test.setText("CHANGE");
            test = (TextView) rootView.findViewById(R.id.stockValue4);
            double change = Double.valueOf(jsonObject.getString("Change"));
            String c = String.format("%.2f", change);
            double changepercent = Double.valueOf(jsonObject.getString("ChangePercent"));
            String cp = String.format("%.2f", changepercent);
            test.setText(c+"(" + cp + "%)");
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage4);
            if (change > 0)
                iw.setImageResource(R.drawable.up);
            if (change < 0)
                iw.setImageResource(R.drawable.down);
            if (change == 0)
                iw.setImageResource(R.drawable.trans);

            test = (TextView) rootView.findViewById(R.id.stockAttribute5);
            test.setText("TIMESTAMP");
            test = (TextView) rootView.findViewById(R.id.stockValue5);
            String daten = jsonObject.get("Timestamp").toString();
            DateTime dt = DateTime.parse(daten, DateTimeFormat.forPattern("EEE MMM dd HH:mm:ss zZZ YYYY"));
            dt = dt.plusHours(-7);
            test.setText(dt.toString("dd MMM YYYY,HH:mm::ss"));
            //test.setText(jsonObject.getString("Timestamp"));
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage5);
            iw.setImageResource(R.drawable.trans);

            test = (TextView) rootView.findViewById(R.id.stockAttribute6);
            test.setText("MARKETCAP");
            test = (TextView) rootView.findViewById(R.id.stockValue6);
            double marketcap = Double.valueOf(jsonObject.getString("MarketCap"));
            if (marketcap > 9999999) {
                marketcap = marketcap / 1000000000;
                String marketcapdisplay = String.format("%.2f", marketcap);
                test.setText(marketcapdisplay + " Billion");
            } else {
                marketcap = marketcap / 1000000;
                String marketcapdisplay = String.format("%.2f", marketcap);
                test.setText(marketcapdisplay + " Million");
            }
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage6);
            iw.setImageResource(R.drawable.trans);

            test = (TextView) rootView.findViewById(R.id.stockAttribute7);
            test.setText("VOLUME");
            test = (TextView) rootView.findViewById(R.id.stockValue7);
            test.setText(jsonObject.getString("Volume"));
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage7);
            iw.setImageResource(R.drawable.trans);

            test = (TextView) rootView.findViewById(R.id.stockAttribute8);
            test.setText("CHANGETYD");
            test = (TextView) rootView.findViewById(R.id.stockValue8);
            change = Double.valueOf(jsonObject.getString("ChangeYTD"));
            c = String.format("%.2f", change);
            changepercent = Double.valueOf(jsonObject.getString("ChangePercentYTD"));
            cp = String.format("%.2f", changepercent);
            assert test != null;
            test.setText(c + "(" + cp + "%)");
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage8);
            if (changepercent > 0)
                iw.setImageResource(R.drawable.up);
            if (changepercent < 0)
                iw.setImageResource(R.drawable.down);
            if (changepercent == 0)
                iw.setImageResource(R.drawable.trans);


            test = (TextView) rootView.findViewById(R.id.stockAttribute9);
            test.setText("HIGH");
            test = (TextView) rootView.findViewById(R.id.stockValue9);
            test.setText("$ "+jsonObject.getString("High"));
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage9);
            iw.setImageResource(R.drawable.trans);

            test = (TextView) rootView.findViewById(R.id.stockAttribute10);
            test.setText("LOW");
            test = (TextView) rootView.findViewById(R.id.stockValue10);
            test.setText("$ "+jsonObject.getString("Low"));
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage10);
            iw.setImageResource(R.drawable.trans);

            test = (TextView) rootView.findViewById(R.id.stockAttribute11);
            test.setText("OPEN");
            test = (TextView) rootView.findViewById(R.id.stockValue11);
            test.setText("$ "+jsonObject.getString("Open"));
            iw = (ImageView) rootView.findViewById(R.id.stockTrendImage11);
            iw.setImageResource(R.drawable.trans);


        } catch (JSONException e) {
            e.printStackTrace();
        }

            //SharedPreferences showfav2 = getSharedPreferences(FAV2, 0);
            //appendedString = showfav2.getString("favcompany", "");

        //String imgurl = "http://chart.finance.yahoo.com/t?s="+supersymbol+"&lang=en-US&width=400&height=300"
        ImageTask imageTask = new ImageTask();

                imageTask.execute("http://chart.finance.yahoo.com/t?s="+tSymbol+"&lang=en-US&width=900&height=630");

        return rootView;
        }

  /*  @Override
    public void onClick(View v) {
        mAttacher = new PhotoViewAttacher(rYahoo);
    }*/


    public class ImageTask extends AsyncTask<String, Void, Bitmap>{


       /* public ImageTask(ImageView bmImage) {
            rYahoo = bmImage;
        }*/

        @Override
        protected Bitmap doInBackground(String... params) {
            String urldisplay = params[0];
            Bitmap finalImage = null;
            try {
                InputStream in = new java.net.URL(urldisplay).openStream();
                finalImage = BitmapFactory.decodeStream(in);
            } catch (Exception e) {
                Log.e("Error", e.getMessage());
                e.printStackTrace();
            }
            return finalImage;
        }
        protected void onPostExecute(final Bitmap result) {
            //ImageButton imageViewYahoo = (ImageButton) rootView.findViewById(R.id.yahooImg);
            imageViewYahoo.setImageBitmap(result);
            imageViewYahoo.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
                     final AlertDialog dialog = builder.create();
                    LayoutInflater inflater = getActivity().getLayoutInflater();
                    View dialogLayout = inflater.inflate(R.layout.alert_dialog, null);

//                    dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
                    dialog.setView(dialogLayout);
                    dialog.show();
                    PhotoViewAttacher mAttacher;
                    ImageView image = (ImageView) dialog.findViewById(R.id.expanded_image);
                    mAttacher = new PhotoViewAttacher(image);
                    image.setImageBitmap(result);

                    dialog.setOnShowListener(new DialogInterface.OnShowListener() {
                        @Override
                        public void onShow(DialogInterface d) {


                            ImageView image = (ImageView) dialog.findViewById(R.id.expanded_image);
                            image.setImageBitmap(result);
                        }
                    });

                }
            });
        }
    }
    }
