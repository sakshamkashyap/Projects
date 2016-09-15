package com.webtech.saksham.stocksearch;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;

@SuppressLint("ValidFragment")
public class ChartsFragment extends Fragment {

    public static final String PREFS = "quotejsonprefs";

    WebView mWebView;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){


        View rootView = inflater.inflate(R.layout.chartsfrag, container, false);
        System.out.println("high charts should print");
        WebView webView = (WebView) rootView.findViewById(R.id.chartsView);
        webView.getSettings().setJavaScriptEnabled(true);
        //webView.loadUrl("file:///android_asset/chart.html");
        webView.loadUrl("http://www-scf.usc.edu/~skashyap/chart.html?Data="+MainActivity.searchText);
        System.out.println("high charts should fin print");
        return rootView;

    }
}
