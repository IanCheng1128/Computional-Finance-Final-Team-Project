#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Stock.h"
#include "SelectRandomStocks.h"
#include <algorithm>
#include <iomanip>
#include <time.h>
#include "Calculation.h"
#include "PlotChart.h"
#include "ExtractData.h"

using namespace std;

int main()
{

    srand((unsigned int)time(NULL));
    vector<string> TickerList;
    
    // get sp500
    vector<double> SPY;
    vector<string> DATE;
    
    cout << "Downloading SPY data..." << endl;
    ExtractSPY(SPY, DATE, "2019-01-10", "2019-08-20");
    //cout <<SPY[1]<<endl;
    vector<double> spy = findSPYReturn(SPY);
    
    StockMap StockList;
    
    // read EPS file
    string path = "EPS.csv";
    EquityDivide test1(path, StockList,TickerList,DATE);
    Group group1 = test1.divide_group(StockList);

    cout << endl << "Downloading Stock data..." << endl;
    Extract(StockList);
    
    Matrix Result(3,vector<vector<double>>(4)) ;

    Result = finalCalculation( group1,spy, StockList);
    
    
    string input;
    int opt;
    bool cont = true;
    while (cont)
    {
        cout << endl << "Please input the number to proceed..." << endl;
        cout << "1: Get historical price data of all stocks" << endl;
        cout << "2: Information for 1 specific stock" << endl;
        cout << "3: AAR, AAR-SD, CAAR, CAAR-SD for one group" << endl;
        cout << "4: Plot CAAR for all three groups" << endl;
        cout << "5: Exit" << endl;
        
        cin >> input;
        while((input.at(0) > '5' || input.at(0) <= '0'))
        {
            cout << "Invalid input! Try Again!" << endl;
            cin >> input;
        }
        
        opt = stoi(input);
        string userticker;
        string choice;
        switch (opt) {
        case 1:
            cout << "Input ticker:";
            cin >> userticker;
            cout << endl;
            if (StockList.find(userticker) != StockList.end())
            {
                int sdate = StockList[userticker].Get_StartDateIndex();
                cout << "Adjust price of stock "<< userticker << endl;
                for (auto price: StockList[userticker].Get_AdjClose()) {
                    cout << DATE[sdate++] <<" " << price << endl;
                    
                }
                
            }
                
            else if (userticker == "SPY")
            {
                int sdate = 0;
                cout << "Adjust price of stock "<< userticker << endl;
                for (auto itr = SPY.begin(); itr != SPY.end(); itr++) {
                    cout << DATE[sdate++] <<" " << *itr << endl;
                }
            }
            else{
                cout << "No Data for this ticker!" << endl;
            }
            break;
                
        case 2:
            cout << "Input ticker:";
            cin >> userticker;
            cout << endl;
            if (StockList.count(userticker)) {
            cout << "Ticker:             " << StockList[userticker].Get_Ticker() << endl;
            cout << "Earnings Call Date: " << StockList[userticker].Get_AnnounceDate() << endl;
            cout << "Actual EPS:         " << StockList[userticker].Get_ActEPS() << endl;
            cout << "Estimated EPS:      " << StockList[userticker].Get_EstEPS() << endl;
            cout << "Surprise:           " << StockList[userticker].Get_Surprise() << endl;
            cout << "Group:              " << StockList[userticker].Get_Group() << endl;
            cout << endl;
        }
            else {
                cout << "No Data for Ticker!" << endl;
            }
            break;
                
        case 3:
            cout << "Choose group: " << endl;
            cout << " 0 for miss, 1 for meet, 2 for beat" << endl;
            int groupselect;
            cin >> groupselect;
            //printstats(groupselect);
            if (groupselect != 1 && groupselect != 2 && groupselect != 0) {
                cout << "Invalid choice!" << endl;
                break;
            }
            cout << "Day    |   AAR     |    AAR-SD     |    CAAR    |    CAAR-SD " << endl;
            for (int i = 0; i < 60; i++) {
                cout << setprecision(0) << i - 30 << "    ";
                cout << setprecision(6) << Result[groupselect][0][i] << "    " << Result[groupselect][1][i] << "    " << Result[groupselect][2][i] << "    " << Result[groupselect][3][i] << endl;
            }
            break;
                
        case 4:
            //call gnuplot and plot.
            plotChart(Result[2][2], Result[1][2], Result[0][2]);
            cout << "Plot, done!" << endl;
            break;
                
        case 5:
            cout << "Are you sure you want to exit (Y to confirm (case sensitive))" << endl;
            cin >> choice;
            if (choice == "Y")
            {
                cout << "Thanks for using!" << endl;
                return 0;
            }
            else
            {
                cout << "Exit Unconfirmed. Not Exiting." << endl;
                break;
            }
            break;
                
        default:
            break;
        };
    };


}
