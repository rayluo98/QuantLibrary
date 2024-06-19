#include<ctime>
#include<cmath>
#include<string>
#include<vector>
#pragma once
using namespace std;

enum TickerType {
    ISIN, CUSIP, SEDOL
};

class Asset{
    private:
        double _price;
        double _quantity;
        double _dividend_rate; // generic cash flow rate (assuming yield... better would have discrete dividend)
        vector<double> _discrete_div;
        vector<time_t> _discrete_div_time;
        std::string _ticker;
        TickerType _tickerType;
        time_t _marketTime;

    public:
        double Price(){return _price;}
        void updatePx(double price) { _price = price; }
        void updateQty(double qty) { _quantity = qty; }
        double Quantity(){return _quantity;}
        void setQuantity(double quantity){_quantity = quantity;}
        std::string Ticker(){return _ticker;}

        //copy constructor
        Asset(const Asset& _asset) {
            _price = _asset._price;
            _quantity = _asset._quantity;
            _ticker = _asset._ticker;
            _tickerType = _asset._tickerType;
            _marketTime = _asset._marketTime;
            _discrete_div= _asset._discrete_div;
            _discrete_div_time = _asset._discrete_div_time;
        }

        //virtual void Update() = 0;
        void Update(double px, double qty) { _price = px; _quantity = qty; };
        Asset() : _price(0), _quantity(0) {}
};
