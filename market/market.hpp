using namespace std; 
#include<ctime>
#include<cstdlib>
#include"../assets/asset.h"
#include <unordered_map>
#include <map>

class Market_History {
public: 
	std::unordered_map<string, Asset> history;
};

class Referential {
public:
	std::map<time_t, Asset> referential_hist;
	string ticker;

	Referential history_trim(time_t t1, time_t t2) {
		std::map<time_t, Asset> new_hist;
		//creates a trim of the history observed
	}
};