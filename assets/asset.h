#include<ctime>
#include<cmath>
#include<string>
#pragma once
using namespace std;

enum TickerType {
    ISIN, CUSIP, SEDOL
};

class Asset{
    private:
        double _price;
        double _quantity;
        std::string _ticker;
        TickerType _tickerType;
        time_t _marketTime;

    public:
        double Price(){return _price;}
        double Quantity(){return _quantity;}
        void setQuantity(double quantity){_quantity = quantity;}
        std::string Ticker(){return _ticker;}

        //virtual void Update() = 0;
        void Update();
        Asset() : _price(0), _quantity(0) {}
};
