package com.webtech.saksham.stocksearch;


public class Bean_Fav {

        private String name = "";
        private String symbol = "";
        private String change = "";
        private String cap = "";
        private String price = "";
        private int color;

        public void setName(String name) {
            this.name = name;
        }

        public String getName() {
            return name;
        }

        public void setSymbol(String symbol) {
            this.symbol = symbol;
        }

        public String getSymbol() {
            return symbol;
        }
        public void setCap(String cap) {
            this.cap = cap;
        }

        public String getCap() {
            return cap;
        }
    public void setChange(String change) {
        this.change = change;
    }

    public String getChange() {
        return change;
    }
    public void setPrice(String price) {
        this.price = price;
    }

    public String getPrice() {
        return price;
    }

    public void setColor(int color) {
        this.color = color;
    }

    public int getColor() {
        return color;
    }
}
