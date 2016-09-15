package com.webtech.saksham.stocksearch;


import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;


public class MyCustomListAdapter extends BaseAdapter {

    Context context;
    public static final String COLORTEXT = "colorprefs";
    private static ArrayList<Bean_Fav> searchArrayList;
    double tempcolor;

    private LayoutInflater mInflater;

    public MyCustomListAdapter(Context context, ArrayList<Bean_Fav> results) {
        searchArrayList = results;
        mInflater = LayoutInflater.from(context);
        this.context = context;
    }

    public int getCount() {
        return searchArrayList.size();
    }

    public Object getItem(int position) {
        return searchArrayList.get(position);
    }

    public long getItemId(int position) {
        return position;
    }

    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder;
        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.fav_item_layout, null);
            holder = new ViewHolder();
            holder.txtName = (TextView) convertView.findViewById(R.id.name_layout);
            holder.txtSymbol = (TextView) convertView.findViewById(R.id.symbol_layout);
            holder.txtCap = (TextView) convertView.findViewById(R.id.cap_layout);
            holder.txtPrice = (TextView) convertView.findViewById(R.id.price_layout);
            holder.txtChange = (TextView) convertView.findViewById(R.id.change_layout);


            convertView.setTag(holder);
        } else {
            holder = (ViewHolder) convertView.getTag();
        }

        holder.txtName.setText(searchArrayList.get(position).getName());
        holder.txtSymbol.setText(searchArrayList.get(position).getSymbol());
        holder.txtCap.setText(searchArrayList.get(position).getCap());
        holder.txtChange.setText(searchArrayList.get(position).getChange());
        holder.txtPrice.setText(searchArrayList.get(position).getPrice());
        holder.txtChange.setBackgroundColor(searchArrayList.get(position).getColor());

        /*if (context != null){
        SharedPreferences showcolor = context.getSharedPreferences(COLORTEXT, 0);
        String changecolor = showcolor.getString("colorjson", "");


        tempcolor = Double.valueOf(changecolor);
            System.out.println(tempcolor);
        if(tempcolor>0) {
            holder.txtChange.setBackgroundColor(Color.GREEN);
        }else if(tempcolor<0){
            holder.txtChange.setBackgroundColor(Color.RED);
        }
        }*/

        return convertView;
    }

    static class ViewHolder {
        TextView txtName;
        TextView txtSymbol;
        TextView txtCap;
        TextView txtPrice;
        TextView txtChange;
    }


}
