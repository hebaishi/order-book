# Order Book C++ Project

## Overview

To create a multi-threaded program that simulates a matching engine maintaining multiple price-time order books.

The program should:
1. Accept input consisting of *orders* and *cancels*;
2. Maintain multiple price-time order books (one order book per trading symbol);
3. Publish *acknowledgments*, *trades*, and *top-of-book* changes.

Multi-threading should be employed to run the matching engine in a
pipeline-like fashion, separating the data ingestion, the matching logic, and
the output into separate threads.

## Requirements

### Input

The program should accept input via UDP. The input will be given in a simple
text format, supporting three event types:
1. New order (`N`);
    - `N, user(int), symbol(string), price(int), qty(int), side(char B or S),
      userOrderId(int)`
2. Cancel order (`C`);
    - `C, user(int), userOrderId(int)`
3. Flush order book (`F`).
    - `F`

### Order Book

Every order book will use a price-time priority system. That means that the
best price should always be matched first, and -- in case of a price tie -- the
time the order was entered will be used as the second priority.

There are two types of orders:
1. *Market orders*: immediately fill against the opposite side, any unmatched
   portion will be canceled ("fill and kill");
2. *Limit orders*: if not matched immediately, will stay in the book in
   price-time priority.

Partial quantity matches are possible.

### Output

Your program should publish output to `stdout', using the following format:

- Order acknowledgement:
    - `{A, userId, userOrderId}`
- Cancel acknowledgement:
    - `{C, userId, userOrderId}`
- Trades (matched orders):
    - `{T, userIdBuy, userOrderIdBuy, userIdSell, userOrderIdSell, price, quantity}`
- Changes in top-of-book per side:
    - `{B, side , price, totalQuantity}`
    - `size` is either `B` or `S`
    - Use `-` for side elimination


## Suggested Approach

The final program will be complex. Break it down step-by-step, in
self-contained small components that are individually testable.

Think about the data, and the transformations it must undertake. Using a more
functional approach will help with simplicity and testability.

Ignore the multithreading requirement at first, and focus on small pieces.

Consider the following example approach:
1. Create a data structure that represents an input event (`std::variant` could
   be a good choice here).
2. Write a function that, given an input event string, returns an input event
   object.
3. Write a few tests, ensure that it works correctly.
4. Create a barebones order book that can accept input events.
5. Implement basic order registration and acknowledgment responses.
6. Test that particular code path.
7. Rinse and repeat... :)

I suggest asking a lot of questions on WhatsApp -- I think it's beneficial to
have an iterative design process. Feel free to modify any part of the
specification if you feel like it's necessary.

Consider also ignoring order cancellation until the very end.

## Example Data

### Example 1

#### Input

```
# build book, TOB = 10/11
N, 1, IBM, 10, 100, B, 1
N, 1, IBM, 12, 100, S, 2
N, 2, IBM, 9, 100, B, 101
N, 2, IBM, 11, 100, S, 102

# hit book on each side, generate trades, TOB = 9/12
N, 1, IBM, 11, 100, B, 3
N, 2, IBM, 10, 100, S, 103

# replenish book on each side, TOB = 10/11
N, 1, IBM, 10, 100, B, 4
N, 2, IBM, 11, 100, S, 104
F
```

#### Output

```
A, 1, 1
B, B, 10, 100
A, 1, 2
B, S, 12, 100
A, 2, 101
A, 2, 102
B, S, 11, 100


A, 1, 3
T, 1, 3, 2, 102, 11, 100
B, S, 12, 100
A, 2, 103
T, 1, 1, 2, 103, 10, 100
B, B, 9, 100

A, 1, 4
B, B, 10, 100
A, 2, 104
B, S, 11, 100
```

### Example 2

#### Input

```
# build book, shallow bid, TOB = 10/11
N, 1, AAPL, 10, 100, B, 1
N, 1, AAPL, 12, 100, S, 2
N, 2, AAPL, 11, 100, S, 102


# hit bid, generate trade, TOB = -/11
N, 2, AAPL, 10, 100, S, 103

# replenish bid, TOB = 10/11
N, 1, AAPL, 10, 100, B, 3
F
```

#### Output

```
A, 1, 1
B, B, 10, 100
A, 1, 2
B, S, 12, 100
A, 2, 102
B, S, 11, 100
A, 2, 103
T, 1, 1, 2, 103, 10, 100
B, B, -, -
A, 1, 3

B, B, 10, 100
```
