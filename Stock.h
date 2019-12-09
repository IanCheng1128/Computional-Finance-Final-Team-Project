//
//  Stock.h
//  Finanal Project
//
//  Created by IanCheng on 12/8/19.
//  Copyright © 2019 NYU FRE 6883. All rights reserved.
//

#ifndef Stock_h
#define Stock_h
#include <vector>
#include <string>
#include <vector>
using namespace std;

class Stock
{
public:
    int StartDateIndex;
    vector<string> AdjClose;
    vector<string> trade_date;
    
    vector<double> Ret;
    vector<double> AR;
    vector<double> CAR;
    
    vector<double> getRet() { return Ret; }
    vector<double> getAR() { return AR; }
    vector<double> getCAR() { return CAR; }
    
    Stock(){StartDateIndex = 0;}
    Stock(int StartDateIndex_){StartDateIndex = StartDateIndex_;}

};

#endif /* Stock_h */
