#include<ctime>
#include<cmath>
using namespace std;

enum tickerType{
    isin, cusip
};

class Asset{
    private:
        double _price;
        double _quantity;
        std::string _ticker;
        tickerType _tickerType;
        time_t _marketTime;

    public:
        double Price(){return _price;}
        double Quantity(){return _quantity;}
        void setQuantity(double quantity){_quantity = quantity;}
        std::string Ticker(){return _ticker};

        virtual void Update() = 0;

};

class Equity: public Asset{
    public:
        Equity(double price, double quantity, std::string ticker="", tickerType type = isin, time_t marketTime = time(0)){
            _price = price; 
            _quantity = quantity;
            _ticker = ticker;
            _tickerType = type;
            _marketTime = marketTime;
        }

        void stockSplit(double mult){
            _quantity *= mult;
            _price /= mult;
        }

        void Update() {}

        ~Equity(){}
}

enum rating{
    AAA
};

class Bond: public Asset{
    private:
        double _yield;
        double _tenor;
        time_t _maturity;
        double _coupon;
        bool _zBondFlag;
    public:
        Bond(time_t maturity, double quantity, double coupon)
}