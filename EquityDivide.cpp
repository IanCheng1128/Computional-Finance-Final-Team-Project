#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "tools.h"
#include "EquityDivide.h"
using namespace std;

EquityDivide::EquityDivide(const string& path, StockMap& StockList, vector<string>& TickerList, vector<string>& Tradedate)
{
    ifstream file;
    file.open(path);
    
    vector<Ticker> id = read_col<vector<Ticker>, Ticker>(file, 0);
    vector<Date> date = read_col<vector<Date>, Date>(file, 1);
    vector<double> actual = read_col<vector<double>, double>(file, 2);
    vector<double> est = read_col<vector<double>, double>(file, 3);
    
    file.close();
    
    //Store the tickers in TickerList
    TickerList = id;
    
    size_t n = id.size();
    int dateindex = 0;
    for(int i=0;i<n;++i)
    {
        double surprise = (actual[i] - est[i]) / abs(est[i]);
        TickerInfo TI = {id[i], date[i]};
        surprise_vec.push_back({surprise, TI});
        //Store information into a Stock class
        Stock temstock;
        temstock.Set_Ticker(id[i]);
        temstock.Set_AnnounceDate(date[i]);
        temstock.Set_EstEPS(est[i]);
        temstock.Set_ActEPS(actual[i]);
        temstock.Set_Surprise(surprise);
        
        //Tradedate starts at Jan/10/2019, we read the date before March
        for(int j = 30; j <=124 ; j++)
        {
            if(Tradedate[j] == date[i])
            {
                dateindex = j-31;
                j = 124;
            }
        }
        temstock.Set_StartDateIndex(dateindex);
        temstock.Set_StartDate(Tradedate[dateindex]);
        temstock.Set_EndDate(Tradedate[dateindex+60]);
        //Key: Stock class
        StockList[id[i]] = temstock;
    }
    
    sort(surprise_vec.begin(), surprise_vec.end());
}


Group EquityDivide::divide_group(StockMap &StockList) const
{
    Group ans(3);
    size_t n = surprise_vec.size();
    int group_size = static_cast<int>(n) / 3;
    vector<string> groupname{"Miss", "Meet", "Beat"};
    /*
    for(int i=0;i<n;++i){
        //Miss group
        if(i < group_size){
            ans[0].push_back(surprise_vec[i].second);
            StockList[surprise_vec[i].second.first].Group = groupname[0];
        }
        //Meet group
        else if(i >= group_size && i < 2 * group_size){
            ans[1].push_back(surprise_vec[i].second);
            StockList[surprise_vec[i].second.first].Group = groupname[1];
        }
        //Beat group
        else{
            ans[2].push_back(surprise_vec[i].second);
            StockList[surprise_vec[i].second.first].Group = groupname[2];
        }
    }
     */
    
    for (int i = 0; i < n; i++) {
        double surprise = surprise_vec[i].first;
        if (surprise < 0.0) {
           ans[0].push_back(surprise_vec[i].second);
            StockList[surprise_vec[i].second.first].Set_Group(groupname[0]);
        }
       else if (surprise < .05) {
            ans[1].push_back(surprise_vec[i].second);
           StockList[surprise_vec[i].second.first].Set_Group(groupname[1]);
       }
       else {
           ans[2].push_back(surprise_vec[i].second);
           StockList[surprise_vec[i].second.first].Set_Group(groupname[2]);
        }
    }

    
    
    return ans;
}


