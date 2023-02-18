using namespace std; 
#include<ctime>
#include<cstdlib>
#include"pricers/unit structures/asset.h"

class Market_History {
public: 
	std::unordered_map<string, 
};

class Referential {
public:
	std::map<time_t, Asset> _hist;
	string _ticker;
	string _refSource;

	Referential history_trim(time_t t1, time_t t2) {
		std::map<time_t, Asset> new_hist;
		//creates a trim of the history observed
	}

	string getSource(const std::string& refSource)
	{return _refSource; _refSource=refSource;}
};