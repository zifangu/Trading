//
// Starter code provided by David Sykes. All four functions have stubs
// Code completed by _____________
//

#include <stdio.h>
#include <assert.h>
#include "besttrade.h"
#include <stdlib.h>


#define DEBUG_MAX_MIN 0


/*
 * IMPLEMENTATION NOTES.
 *    1. A formal parameter name of _price_ indicates the argument is an array having n+1
 *      elements but only elements price[1] through price[n] are used.
 *    2. A formal parameter name of _a_ indicates the argument is an array having n elements
 *       a[0] through a[n-1].
 */


static int max_price_day_nbr(int *price, int n);
    // Return the index of the largest value in array a.


static int min_price_day_nbr(int *price, int n);
    // Find the index of the minimum value in an array

void print_prices(int *price, int n);
    // Print an array containing prices


static void print_array(int *a, int n);
// Find the index of the maximum value in an array
// *** This array uses zero-based indexing. ***


struct Trade trade_bf(int *price, int n) {
    assert(price && n > 0);  // Check precondition

    // Brute force algorithm
    struct Trade result;

    int buy_index = 1;
    int sell_index = n;
    int best_profit = price[sell_index] - price[buy_index];

    // TODO -- replace stub code

    for (int i = 1; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            int temp = price[j] - price[i];
            if (temp > best_profit) {
                best_profit = temp;
                buy_index = i;
                sell_index = j;
            }
        }
    }

    result.buy_day_nbr = buy_index;
    result.sell_day_nbr = sell_index;
    result.profit = best_profit;
    return result;
}


// function  trade(prices)
//    if  prices.length = 1
//        return  0
//    former ← prices.first_half
//    latter ← prices.last_half
//    case3 ← max(latter) - min(former)
//    return  max(trade(former), trade(latter), case3)

struct Trade trade_dc(int *price, int n) {
    assert(price && n > 0);  // Check precondition

    struct Trade result;
//        result.buy_day_nbr = 1;
//        result.sell_day_nbr = 1;
//        result.profit = 0;

    if (n == 1) {
        return result ;
    }

    int length = n / 2;
    printf("%s%i%s", "length: ", length, "\n");
    int *former = malloc(length * sizeof(int));
    int *latter = malloc(length * sizeof(int));
    for (int i = 0; i < length; i++) {
        printf("%s%i%s", "price: ", price[i], "\n");
        former[i] = price[i];
        latter[i] = price[length + i];
        printf("%s%i%s", "former: ", former[i], "\n");
        printf("%s%i%s", "latter: ", latter[i], "\n");
    }
    int maximum;
    int case3 = latter[max_price_day_nbr(latter, length)] - former[min_price_day_nbr(former, length)];
    if ((trade_dc(former, length).profit > trade_dc(latter, length).profit) && (trade_dc(former, length).profit > case3)) {
        maximum = trade_dc(former, length).profit;
    } else if (trade_dc(former, length).profit > case3) {maximum = trade_dc(latter, length).profit;}
    else {maximum = case3;}







    result.buy_day_nbr = former[min_price_day_nbr(former, length)];
    result.sell_day_nbr = latter[max_price_day_nbr(latter, length)];
    result.profit = maximum;
    return result;
}



//function  trade_dp(P)
//    B[1] ← 1
//    sell_day ← 1
//    best_profit ← 0
//     
//    for each n from 2 to P.length
//        if  P[n] < P[B[n-1]]
//            B[n] ← n
//        else
//            B[n] ← B[n-1]
//     
//        profit ← P[n] - P[B[n]]
//        if  profit > best_profit
//            sell_day ← n
//            best_profit ← profit
//     
//    return  (sell_day, B[sell_day])

struct Trade trade_dp(int *price, int n) {
    // Dynamic programming algorithm
    assert(price && n > 0);  // Check precondition

    struct Trade result;

    int best_day[n];
    best_day[1] = 1;
    int sell_index = 1;
    int best_profit = 0;

    // TODO -- replace stub code
    for (int i = 2; i < n+1; i++) {
        if (price[i] < price[best_day[i - 1]]) {
            best_day[i] = i;
        } else {best_day[i] = best_day[i - 1];}
//        printf("%s%i%s", "day looking at: ", i, "\n");
//        printf("%s%i%s", "best_day[i] = : ", best_day[i], "\n");
        int temp = price[i] - price[best_day[i]];
//        printf("%s%i%s", "profit: ", temp, "\n");
        if (temp > best_profit) {
//            printf("%s%i%s","current best buy day: ", best_day[sell_index], "\n");
//            printf("%s%i%s", "current best sell day: ", i, "\n");
            sell_index = i;
            best_profit = temp;
        }
    }

        result.buy_day_nbr = best_day[sell_index];
        result.sell_day_nbr = sell_index;
        result.profit = best_profit;
        return result;
}

/*
    function trade_kadane(prices):
        sell_day ← 1
        buy_day ← 1
        B ← 1
        best_profit ← 0

        for each  s from 2 to prices.length
            if  prices[s] < prices[buy_day]
                B ← s
            profit ← prices[s] - prices[B]
            if  profit ≥ best_profit
                sell_day ← s
                buy_day ← B
                best_profit ← profit
        return  (sell_day, buy_day)

Excerpt From: Wladston Ferreira Filho. “Computer Science Distilled.” Apple Books.
 *
 */

struct Trade trade_kadane(int *price, int n) {
    // Kadane's algorithm
    assert(price && n > 0);  // Check precondition

    struct Trade result;

    int buy_index = 1;
    int sell_index = 1;
    int B = 1;
    int best_profit = 0;

    // TODO -- replace stub code
    for (int i = 2; i < n; i++) {
        if (price[i] < price[buy_index]) {
            B = i;
        }
        int temp = price[i] - price[B];
        if (temp > best_profit) {
            sell_index = i;
            buy_index = B;
            best_profit = temp;
        }
    }

    result.buy_day_nbr = buy_index;
    result.sell_day_nbr = sell_index;
    result.profit = best_profit;
    return result;
}


// NO NEED TO CHANGE ANY OF THE CODE THAT FOLLOWS


static void print_array(int *a, int n) {
    assert(a && n > 0);  // Check precondition
    printf("%d", a[0]);
    for (int i = 1; i < n; ++i) {
        printf(", %d", a[i]);
    }
}

void print_prices(int *price, int n) {
    assert(price && n > 0);  // precondition check
    printf("[");
    print_array(price + 1, n);
    printf("]");
}

static int max_price_day_nbr(int *price, int n) {
    assert(price && n > 0);  // precondition check

    if (DEBUG_MAX_MIN) {
        // Display this function's arguments
        printf("    - max_price_day_nbr([");
        print_prices(price, n);
        printf("], %d) -> ", n);
        fflush(stdout);
    }

    int index_max_elt_value = 1;
    int max_elt_value = price[1];
    for (int i = 2; i <= n; ++i) {
        if (price[i] > max_elt_value) {
            max_elt_value = price[i];
            index_max_elt_value = i;
        }
    }

    if (DEBUG_MAX_MIN) {
        printf("%d (a[%d] -> %d)\n", index_max_elt_value, index_max_elt_value, price[index_max_elt_value]);
    }
    return index_max_elt_value;
}

static int min_price_day_nbr(int *price, int n) {
    assert(price && n > 0);  // precondition check

    if (DEBUG_MAX_MIN) {
        // Display this function's arguments
        printf("    - min_price_day_nbr([");
        print_prices(price, n);
        printf("], %d) -> ", n);
        fflush(stdout);
    }

    int index_min_elt_value = 1;
    int min_elt_value = price[1];
    for (int i = 2; i <= n; ++i) {
        if (price[i] < min_elt_value) {
            min_elt_value = price[i];
            index_min_elt_value = i;
        }
    }
    if (DEBUG_MAX_MIN) {
        printf("%d (a[%d] -> %d)\n", index_min_elt_value, index_min_elt_value, price[index_min_elt_value]);
    }
    return index_min_elt_value;
}
