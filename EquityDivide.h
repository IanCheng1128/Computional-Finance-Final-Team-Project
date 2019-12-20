#ifndef EquityDivide_h
#define EquityDivide_h

#include <string>
#include <vector>
#include <map>
#include "Stock.h"
using namespace std;

typedef string Ticker;
typedef string Date;
typedef pair<Ticker, Date> TickerInfo;
//Store the ticker and announcement date
typedef vector<vector<TickerInfo>> Group;
//Store the ticker and stock class in a map
typedef map<string,Stock> StockMap;

class EquityDivide{
protected:
    vector<pair<double, TickerInfo> > surprise_vec;
public:
    EquityDivide(const string& path, StockMap& StockList, vector<string>& TickerList, vector<string>& Tradedate);
    Group divide_group(StockMap &StockList) const;
};


#endif /* EquitDivide_h */
