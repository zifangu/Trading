/*
 * This program solves the Best Trade problem.
 *
 * The daily prices generated dynamically
 */


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <getopt.h>
#include <assert.h>

#include "besttrade.h"

#define TRIALS 10000


void test_trade(struct Trade (*f)(int *price, int n), int *price, int n, const char *heading, int nbr_trials);
    // Call the function with the prices given. The array is zero-based, but price[0] is ignored.

static void generate_prices(int *price, int n);
    // Generate random prices for n days.
    // price[i] contains the price on day i, i = 1, 2, ..., n.
    // (The array contains n+ 1 elements. price[0] is set to -1.)

static void shuffle(int *price, int n);
    // Shuffle an array having n elements.


int verbose_flag = 0;
static int randomize_flag = 0;
static int shufflehalves_flag = 0;
static int bookdata_flag = 0;


int main(int argc, char **argv) {

    if (argc == 1) {
        // There are too few arguments. At least two prices must be provided
        fprintf(stderr, "Too few command line arguments.\n");
        fprintf(stderr, "Use --help for options.\n");
        exit(1);
    }


    // Process any command line options (--trials=<number>)
    // Adapted from // https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
    int trials = 0;  // default value for the number of trials
    int days = 2;    // default value for number of trading days

    static struct option long_options[] = {
            {"verbose",   no_argument,       &verbose_flag,           1},
            {"randomize", no_argument,       &randomize_flag,         1},
            {"shufflehalves", no_argument,       &shufflehalves_flag, 1},
            {"bookdata",  no_argument,       &bookdata_flag,          1},
            {"help",      no_argument,       0,                       '?'},
            {"days",      required_argument, 0,                       'd'},
            {"trials",    required_argument, 0,                       't'},
            {0, 0,                           0,                       0}
    };

    int option_index = 0;
    int c;

    c = getopt_long(argc, argv, "t:", long_options, &option_index);

    while (c != -1) {

//        if (long_options[option_index].flag != 0)
//            break;
//        printf ("option %s", long_options[option_index].name);
        switch (c) {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;
            case '?':
                fprintf(stderr, "Usage: '%s' [--help] [--verbose] [--randomize] [--bookdata] "
                                "[--trials=<n>] [--days=<n>]\n\n", argv[0]);
                fprintf(stderr, "    --help           Show command options.\n");
                fprintf(stderr, "    --verbose        Show more output about a run options.\n");
                fprintf(stderr, "    --randomize      Seed rand() before generating price data.\n");
                fprintf(stderr, "    --bookdata       Use prices 27, 53, 7, 25, 33, 2, 32, 47, 43.\n");
                fprintf(stderr, "    --trials=<n>     Time the function executions, calling each n times.\n");
                fprintf(stderr, "                     If n=0 (default) do not time calls.\n");
                fprintf(stderr, "    --days=<n>       Generate n days of price data, default days=2.\n");
                fprintf(stderr, "    --shufflehalves  Randomize prices such that the lowest price is in the\n");
                fprintf(stderr, "                     former half and the highest is in the latter half.\n");
                fprintf(stderr, "                     Default: shuffle all of the price data.\n");
                exit(1);
                break;
            case 't':
                if (optarg) {
                    trials = atoi(optarg);
                    if (trials < 0) {
                        fprintf(stderr, "Invalid number.\n");
                        exit(7);
                    }
                }
                break;
            case 'd':
                if (optarg) {
                    days = atoi(optarg);
                    if (days < 2) {
                        fprintf(stderr, "At least 2 days must be specified.\n");
                        exit(5);
                    }
                }
                break;
            default:
                fprintf(stderr, "Unknown option %s. Ignored.\n", long_options[option_index].name);
        }

        c = getopt_long(argc, argv, "t:", long_options, &option_index);
    }

    if (verbose_flag) {
        if (bookdata_flag) {
            printf("bookdata, %d trials%s%s\n", trials, verbose_flag ? " (verbose)" : "",
                   randomize_flag ? " (randomize)" : "");
        }
        else {
            printf("%d days, %d trials%s%s\n", days, trials, verbose_flag ? " (verbose)" : "",
                   randomize_flag ? " (randomize)" : "");
        }
    }

    if (randomize_flag) {
        // Seed the random number generator with the time
        if (verbose_flag) {
            printf("Seeding rand().\n");
        }
        srand(time(NULL));
    }

    // Set up prices
    int n;
    int prices[bookdata_flag ? 10 : (days + 1)];
    if (bookdata_flag) {
        // Use the values from the book's example on page 53
        int book_prices[10] = {-1, 27, 53, 7, 25, 33, 2, 32, 47, 43};
        memcpy(prices, book_prices, 10 * sizeof(int));
        n = 9;
    }
    else {
        n = days;
        generate_prices(prices, n);
    }



    // Call each of the four functions
    test_trade(trade_bf, prices, n, "Calling trade_bf()", trials);
    test_trade(trade_dc, prices, n, "Calling trade_dc()", trials);
    test_trade(trade_dp, prices, n, "Calling trade_dp()", trials);
    test_trade(trade_kadane, prices, n, "Calling trade_kadane()", trials);

    return 0;
}


void test_trade(struct Trade (*f)(int *price, int n), int *price, int n, const char *heading, int nbr_trials) {

    // Print a heading
    printf("\n\n%s:\n", heading);

    // Print the array of prices
    printf("    Input: ");
    print_prices(price, n);
    printf("\n");

    struct Trade result = (*f)(price, n);
    printf("    Output: BUY Day %d, SELL Day %d, PROFIT: %d\n",
           result.buy_day_nbr, result.sell_day_nbr, result.profit);

    if (nbr_trials) {
        // Call the function repeatedly and display the total and average running times
        clock_t begin = clock();
        for (int i = 0; i < nbr_trials; ++i) {
            result = (*f)(price, n);
        }
        clock_t end = clock();
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("    Running time for %d calls: %.3f seconds (Average %f)", nbr_trials, time_spent,
               time_spent / nbr_trials);
    }

}

static void generate_prices(int *price, int n) {
    // Generate price data that increases a variable amount each day
    assert(n > 0);
    int delta = 9;
    price[0] = -1;  // NOT USED
    price[1] = (rand() % delta) + 1;
    for (int i = 2; i <= n; ++i) {
        price[i] = price[i - 1] + rand() % delta;
    }
    price[0] = -(price[n] + 1);    // Help guard against accidental use by using a "very negative" number

    if (verbose_flag) {
        printf("Generated prices for %d days\nLowest: %d, Highest: %d\n", n, price[1], price[n]);
    }

    // Now shuffle in one of two ways: the whole array or the values in two halves
    if (!shufflehalves_flag) {
        // Shuffle the whole array
        shuffle(price, n);
        if (verbose_flag) {
            printf(" (all values shuffled)\n\n");
        }
    }
    else {
        // Tails: shuffle each of the two halves
        int best_profit = price[n] - price[1];
        int m = n / 2;
        shuffle(price, m);
        shuffle(price + m, n - m);  // includes a [0] element
        if (verbose_flag) {
            printf(" (former [1-%d] and latter [%d-%d] halves shuffled - best profit %d)\n\n", m, m + 1, n, best_profit);
        }
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(int *price, int n) {
    if (verbose_flag) {
        printf("SHUFFLE ");
        print_prices(price, n);
        printf("\n");
    }
    for (int i = n; i > 2; i--) {
        int j = (rand() % i) + 1; // random number in range 1 .. i
        // printf("Swapping %d and %d\n", price[i], price[j]);
        swap(&price[i], &price[j]);
    }
}
