#ifndef Stock_hpp
#define Stock_hpp

#include <stdio.h>
#include <string>
#include <vector>
using namespace std;


class Stock
{
private:
    int StartDateIndex;
    string Ticker;
    string Group;
    vector<double> AdjClose;
    vector<double> Return;
    vector<double> AbnormalReturn;
    string StartDate;
    string EndDate;
    string AnnounceDate;
    double EstEPS;
    double ActEPS;
    double Surprise;

public:
    Stock(){StartDateIndex = 0;}
    Stock(int StartDateIndex_){StartDateIndex = StartDateIndex_;}
    
    void Set_AdjClose(vector<double> AdjClose_);
    void Set_Return(double ret);
    void Set_AbnormalReturn(double abr);
    void Set_Ticker(string t);
    void Set_Group(string group);
    void Set_AnnounceDate(string date);
    void Set_EstEPS(double eeps);
    void Set_ActEPS(double aeps);
    void Set_Surprise(double surprise);
    void Set_StartDateIndex(int sdi);
    void Set_StartDate(string sd);
    void Set_EndDate(string ed);
    
    vector<double> Get_AdjClose();
    vector<double> Get_Return();
    vector<double> Get_AbnormalReturn();
    int Get_StartDateIndex();
    string Get_StartDate();
    string Get_EndDate();
    string Get_Ticker();
    string Get_Group();
    string Get_AnnounceDate();
    double Get_ActEPS();
    double Get_EstEPS();
    double Get_Surprise();
    

};


#endif /* Stock_hpp */
