/*
 * These functions solve the Best Trade Problem:
 *
 *     You have the daily prices of gold for a interval of time. You want to find two days
 *     in this interval such that if you had bought then sold gold at those dates, you'd
 *     have made the maximum possible profit.
 *
 *     Excerpt From: Wladston Ferreira Filho. “Computer Science Distilled.” Apple Books.
 *
 * The input to a function is an array price[n], where price[0] contains the price of gold
 * on day 1, price[1] is the price on day 2, and so on up to price[n-1].
 *
 * The output is a structure that provides the two day number indexes.
 */


#ifndef BEST_TRADE_PROJECT_BESTTRADE_H
#define BEST_TRADE_PROJECT_BESTTRADE_H

struct Trade {
    int buy_day_nbr;
    int sell_day_nbr;
    int profit;
};

extern int verbose_flag;  // Set to 1 if --verbose option is in effect

struct Trade trade_bf(int *price, int n);
    // Determine the buy and sell days that maximize profit.
    // price[i] contains the price on day i, i = 1, 2, ..., n.
    // (The array contains n+ 1 elements. price[0] is ignored.)

struct Trade trade_dc(int *price, int n);
    // Determine the buy and sell days that maximize profit.
    // price[i] contains the price on day i, i = 1, 2, ..., n.
    // (The array contains n+ 1 elements. price[0] is ignored.)

struct Trade trade_dp(int *price, int n);
    // Determine the buy and sell days that maximize profit.
    // price[i] contains the price on day i, i = 1, 2, ..., n.
    // (The array contains n+ 1 elements. price[0] is ignored.)

struct Trade trade_kadane(int *price, int n);
    // Determine the buy and sell days that maximize profit.
    // price[i] contains the price on day i, i = 1, 2, ..., n.
    // (The array contains n+ 1 elements. price[0] is ignored.)

void print_prices(int *price, int n);
    // Provided for debugging, but better placed in another header file


#endif //BEST_TRADE_PROJECT_BESTTRADE_H
