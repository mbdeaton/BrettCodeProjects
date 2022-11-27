#!/usr/bin/env python3
# Brett Deaton - Mar 2022
"""Daily Coding Problem 138.

Find the minimum number of coins to make n cents (using American
denominations 1, 5, 10, 25).
"""


def num_coins(n_cents, denoms=(1, 5, 10, 25)):
    """Count the number of coins needed to sum to n cents.

    Args:
        n_cents: int number of cents to sum to
        denoms: tuple of coin values in any order, default US coins

    Returns:
        min number of coins of all denoms needed to sum to n_cents
    """
    # coin_record registers the number of each coin needed
    # in order from large to small denominations
    coin_record = {c:0 for c in sorted(denoms, reverse=True)}

    # algorithm: for each coin size (starting from largest)
    # 1. calculate number of that coin that gives <= n_cents
    # 2. subtract that money from n_cents and continue with remainder
    for coin in coin_record:
        coin_record[coin] += n_cents // coin
        n_cents -= coin*coin_record[coin]
 
    return sum(coin_record.values())


# unit tests
if __name__ == "__main__":
    # labeled tuples of input and expected values: (input, expected)
    tests = {"zero_test": (0, 0),
             "all_quarters_test": (150, 6),
             "all_pennies_test": (4, 4),
             "99_cent_candy_bar_test": (99, 9),
             "fancy_dinner_and_a_bottle_of_wine_test": (13160, 527),
            }
    message = {True: "passed", False: "FAILED"}
    print("Test_Name Input Output Success")
    for name, value in tests.items():
        calculated_num_coins = num_coins(value[0])
        success = calculated_num_coins == value[1]
        print(f"{name} {value[0]} {calculated_num_coins} {message[success]}")
