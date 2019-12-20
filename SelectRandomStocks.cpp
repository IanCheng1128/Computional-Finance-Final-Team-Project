#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "EquityDivide.h"
#include "SelectRandomStocks.h"
using namespace std;

void StockShuffler::ShuffleStocks() {
    random_shuffle(stock_groups[0].begin(), stock_groups[0].end() );
    random_shuffle(stock_groups[1].begin(), stock_groups[1].end() );
    random_shuffle(stock_groups[2].begin(), stock_groups[2].end() );
}

void StockShuffler::Get30StocksPerGroup(Group &chosen_stocks) {
//we assume 'chosen_stocks' starts out as size 3, 30;
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j < 30; j++) {
            //chosen_stocks[i][j] = stock_groups[i][j];
            chosen_stocks[i].push_back(stock_groups[i][j]);
        }
    }
}
