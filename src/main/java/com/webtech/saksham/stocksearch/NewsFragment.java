package com.webtech.saksham.stocksearch;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;


public class NewsFragment extends Fragment {
    WebView mWebView;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){


        View rootView = inflater.inflate(R.layout.newsfrag, container, false);
        System.out.println("high charts should print");
        WebView webView = (WebView) rootView.findViewById(R.id.newsView);
        webView.getSettings().setJavaScriptEnabled(true);
        //webView.loadUrl("file:///android_asset/chart.html");
        webView.loadUrl("http://www-scf.usc.edu/~skashyap/newsfeed.html?NewsFeed="+MainActivity.searchText);
        System.out.println("high charts should fin print");
        return rootView;

    }
}
