#include<ctime>
#include<cmath>
#include<string>
#include<vector>
#include<map>
#pragma once
using namespace std;

enum TickerType {
    ISIN, CUSIP, SEDOL
};

class Asset{
    private:
        double _price=0;
        double _quantity=0;
        double _dividend_rate=0; // generic cash flow rate (assuming yield... better would have discrete dividend)
        vector<double> _discrete_div;
        vector<time_t> _discrete_div_time;
        std::string _ticker;
        TickerType _tickerType;
        time_t _marketTime;

    public:
        double Price(){return _price;}
        auto getDivCurve(){
            std::map<time_t, double> resDiv;
            if (_discrete_div.size() != _discrete_div_time.size()) //throw error
                cout << "you need to add error logs properly later you bum" << endl;
            for (size_t ct = 0; ct < _discrete_div.size(); ++ct) {
                resDiv.insert(make_pair(_discrete_div_time[ct], _discrete_div[ct]));
            }
            return resDiv;
        }

        double getDivRate(bool recal = false) {
            if (recal) calcDivRate();
            return _dividend_rate;
        }
        void calcDivRate() {
            return;
        }
        void updatePx(double price) { _price = price; }
        void updateQty(double qty) { _quantity = qty; }
        void updateDiv(double div) { _dividend_rate = div; }
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
