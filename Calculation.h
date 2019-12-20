#ifndef Calculation_h
#define Calculation_h

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "tools.h"
#include "Stock.h"
#include "EquityDivide.h"
#include <map>

//  A map that stores the abnormal return of each stock in a vector with its name.
typedef map<string,vector<double>> Ret;

// The data structure that used to store AAR and CAAR.
typedef vector <double> Vector;

// The data structure that is used to store the final result(AAR, AARSD, CAAR, CAARSD) of three groups - Miss, Meet, Beat
typedef vector<vector<vector<double>>> Matrix;

// The data structure that is used to store all the data of AAR and CAAR in order to do the calculation of AARSD and CAARSD
typedef vector<vector<double>> Container;


// This will generate the return of SPY data.
vector<double> findSPYReturn(vector<double> spy)
{
    vector<double> SPYRETURN;
    
    for ( int i = 0; i < spy.size()-1; i ++)
    {
        double SPY3 = (spy[i + 1] - spy[i])/spy[i];
        SPYRETURN.push_back(SPY3);
    }
    return SPYRETURN;
}

//First, it will calculate the return of each stock, and then generate the abonormal return for each stock for each day.
// Third, it will store the abnormal return of each stock into the Stock structure in the StockMap, and generate another map just with the stock name and abnormal return.
void calculationOfReturn (Ret &abnormalReturn, StockMap stocklist,vector<double>spy )
{
    for (StockMap:: iterator it = stocklist.begin(); it != stocklist.end();it ++){
        vector<double> abnormal;
        vector<double> stockPrice = it -> second.Get_AdjClose();
        string stockName = it->first;
        
        int startDate = it -> second.Get_StartDateIndex();
        for (int i = 0; i < 60 ; i ++)
        {
            double first = stockPrice[i];
            double second = stockPrice[i+1];
            
            double result = (second - first)/first;
            it -> second.Set_Return(result);
  
            result = result - spy[startDate + i];
            it -> second.Set_AbnormalReturn(result);
            abnormal.push_back(result);
        }
        abnormalReturn.insert({stockName,abnormal});
    };
}

// Calculate the AAR based on each day.
Vector calculationOfAAR(Ret abnormalReturn)
{
    Vector calculation(60,0);
    for (auto it = abnormalReturn.begin(); it != abnormalReturn.end();it ++){
        for ( int i = 0; i < 60 ; i ++){
            calculation[i] += it->second[i]/30;
        }
    }
    return calculation;
}

// Using the AAR to calculate the CAAR based on each day.
Vector calculationOfCAAR(Vector calculation){
    Vector calculationCAAR(60,0);
    calculationCAAR[0] = calculation [0];
    for(int i = 0; i < 60; i ++){
        calculationCAAR[i + 1] = calculationCAAR[i] + calculation[i + 1];
    }
    return calculationCAAR;
}

// The operator is used to calculate adding to vectors.
Vector operator +(Vector &V1, const Vector &V2){
    for (int i  = 0; i <60; i++){
    // This is for calculating average AAR and CAAR, you have 60 days of data for each stock, and 30 stocks for each group.
        V1[i] = V1[i] + V2[i]/30;
    }
    return V1;
}

// The operator used to calculate container( matrix format(30*60))and vector.
Vector operator+( Container &V1, Vector &V2){
    for (int i = 0; i < 60; i++){
        for (int j = 0; j< 30; j++){
        // This is used to calculate the AARSD and CAARSD
            V2[i] = V2[i]+V1[j][i]/30;
        }
    }
    return V2;
}

// This operator is used to calcuate the subtraction between container and vector.
Container operator -(Container &V1, const Vector &V2){
    for (int i  = 0; i <60; i++){
        for (int j = 0; j < 30; j++){
            V1[j][i] = V1[j][i] - V2[i];
        }
    }
    return V1;
}

// This operator is used to do the matrix square.
Container operator ^(Container &V1, Container &V2)
{
    for (int i  = 0; i <60; i++){
        for (int j = 0; j <30; j++){
            V1[j][i] = V2[j][i] * V2[j][i];
        }
    }
    return V1;
}

// This is used to calculate the square root of the vector.
Vector squareRootOperator(Vector &V1)
{
    for (int i  = 0; i <60; i++){
        V1[i] = sqrt(V1[i]);
    }
    return V1;
    
}

// This is used for generated the AAR, AARSD, CAAR, CAARSD for three groups; simulated for 30 times
Matrix finalCalculation(Group group1,vector<double>spy, StockMap StockList)
{
    vector<double> original(60,0.0);
    Vector AAR1 (60,0.0);
    Vector AARSD1(60,0.0);
    Vector AAR2 (60,0.0);
    Vector AARSD2(60,0.0);
    Vector AAR3 (60,0.0);
    Vector AARSD3(60,0.0);
    Container temp11(30,original);
    Container temp21 (30,original);
    Container temp12(30,original);
    Container temp22 (30,original);
    Container temp31(30,original);
    Container temp32 (30,original);
    Vector CAAR1(60,0.0);
    Vector CAARSD1(60,0.0);
    Vector CAAR2(60,0.0);
    Vector CAARSD2(60,0.0);
    Vector CAAR3(60,0.0);
    Vector CAARSD3(60,0.0);
    Matrix Result(3,vector<vector<double>>(4));
    
    //The calculation of all the abonormal return for all stock.
    Ret abnormalReturn;
    calculationOfReturn(abnormalReturn, StockList, spy);
     
    // simulate for thirty times.
    for (int j = 0; j < 30 ;j++){
        // shuffle the stocks.
        StockShuffler shuffle = StockShuffler(group1);
        // Create 3 groups for putting the sampling stocks.
        Group group2(3);
        shuffle.ShuffleStocks();
        // Get thirty stocks for each group. and all of them are put into group.
        shuffle.Get30StocksPerGroup(group2);
       
        //This will be the abnormal return for miss group.
        Ret abnormalReturn1;
        //This will be the abnormal return for meet group.
        Ret abnormalReturn2;
        //This will be the abnormal return for the beat group.
        Ret abnormalReturn3;

        // We need to divide the stock in the abnormal return map(abnormalReturn) into three groups based on the group data we have in group2.
        for (int i = 0; i<3; i++){
            for (auto it = group2[i].begin(); it != group2[i].end(); it++){
                  if (abnormalReturn.find(it->first) != abnormalReturn.end()){
                      string name = it->first;
                        if (i ==0){
                            abnormalReturn1.insert({name,abnormalReturn[name]});
                      } else if (i ==1){
                            abnormalReturn2.insert({name,abnormalReturn[name]});
                      } else{
                            abnormalReturn3.insert({name,abnormalReturn[name]});
                           }
                       }
                   }
               }
                  
            
        
        //Calculate the average abnormal return of each group.
        Vector averageAbnormalReturn1;
        averageAbnormalReturn1 = calculationOfAAR(abnormalReturn1);
        Vector averageAbnormalReturn2;
        averageAbnormalReturn2 = calculationOfAAR(abnormalReturn2);
        Vector averageAbnormalReturn3;
        averageAbnormalReturn3 = calculationOfAAR(abnormalReturn3);
        
        // This is the container that contains the abnormal return of each stock in every simulation(30*60)
        temp11[j] =averageAbnormalReturn1;
        temp21[j] =averageAbnormalReturn2;
        temp31[j] =averageAbnormalReturn3;
        
        // These are the calculation of average AAR for three groups(30 simulations).
        AAR1 = operator+ (AAR1 ,averageAbnormalReturn1);
        AAR2 = operator+ (AAR2 ,averageAbnormalReturn2);
        AAR3 = operator+ (AAR3 ,averageAbnormalReturn3);
                  
        //This is the calculation of average CAAR for 3 groups (30 simulation).
        Vector culmulativeAbnormalReturn1;
        Vector culmulativeAbnormalReturn2;
        Vector culmulativeAbnormalReturn3;
        culmulativeAbnormalReturn1 = calculationOfCAAR(averageAbnormalReturn1);
        culmulativeAbnormalReturn2 = calculationOfCAAR(averageAbnormalReturn2);
        culmulativeAbnormalReturn3 = calculationOfCAAR(averageAbnormalReturn3);
       
        //This is the container that contains the CAAR of each stock in every simulation
        temp12 [j] =culmulativeAbnormalReturn1;
        temp22 [j] =culmulativeAbnormalReturn2;
        temp32 [j] =culmulativeAbnormalReturn3;
        
        //These are the calculation of average CAAR for three groups (30 simulations)
        CAAR1 = operator+(CAAR1, culmulativeAbnormalReturn1);
        CAAR2 = operator+(CAAR2, culmulativeAbnormalReturn2);
        CAAR3 = operator+(CAAR3, culmulativeAbnormalReturn3);
            
    }
    
    //SD formula: sqrt(sum([x_i - average x)^2]/n)
    temp11 = temp11 - AAR1;
    temp11 = temp11 ^ temp11;
    AARSD1 = operator+(temp11, AARSD1);
    AARSD1 = squareRootOperator(AARSD1);
    
    temp21 = temp21 - AAR2;
    temp21 = temp21 ^ temp21;
    AARSD2 = operator+(temp21, AARSD2);
    AARSD2 = squareRootOperator(AARSD2);
    
    temp31 = temp31 - AAR3;
    temp31 = temp31 ^ temp31;
    AARSD3 = operator+(temp31, AARSD3);
    AARSD3 = squareRootOperator(AARSD3);
              
    temp12 = operator-( temp12, CAAR1);
    temp12 = operator ^(temp12,temp21);
    CAARSD1 = operator+(temp12,CAARSD1);
    CAARSD1 = squareRootOperator(CAARSD1);
    
    temp22 = operator-( temp22, CAAR2);
    temp22 = operator ^(temp22,temp22);
    CAARSD2 = operator+(temp22,CAARSD2);
    CAARSD2 = squareRootOperator(CAARSD2);
    
    temp32 = operator-( temp32, CAAR3);
    temp32 = operator ^(temp32,temp32);
    CAARSD3 = operator+(temp32,CAARSD3);
    CAARSD3 = squareRootOperator(CAARSD3);
    
    // Assign the result of 30 simulations for AAR, AARSD, CAAR, CAARSD into the matrix that is created.(The form of the matrix will be 3*4*60).
    Result[0][0]= AAR1;
    Result[0][1] = AARSD1;
    Result[0][2] = CAAR1;
    Result[0][3] = CAARSD1;
       
    Result[1][0] = AAR2;
    Result[1][1] = AARSD2;
    Result[1][2] = CAAR2;
    Result[1][3] = CAARSD2;
       
    Result[2][0] = AAR3;
    Result[2][1] = AARSD3;
    Result[2][2] = CAAR3;
    Result[2][3] = CAARSD3;
    return Result;
    
}

#endif /* Calculation_h */
