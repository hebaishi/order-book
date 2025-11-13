1. I like the suggestion of using a variant to parse the message, so we would have:

```cpp
using OrderId = int;
namespace event {
enum class Side {
    Buy,
    Sell
};
struct New {
  int user;
  std::string symbol;
  int price;
  int quantity;
  Side side;
};
struct Cancel {
    int user;
    OrderId user_order_id;
};

struct Flush{};

}
using Event = std::variant<event::New, event::Cancel, event::Flush>;

std:optional<Event> Parse(std::string_view buffer);
```

It would be nice to switch to a binary format at some stage as well to speed up the
parsing process. We could also speed it up by removing the string from the new struct,
making the New messaage fixed-size. That can be done later.

2. I then need to represent the list of bids and asks, and we can even sort
   them the same way, and have an abstraction for the notion of 'best' that
   gives us the highest bid and lowest ask (a template specialized on the Side
   enum should work here).
   Ideally we want:
    1. Fast insertion of bids and asks, maintaining the best price for each
    2. A queue per price (atleast conceptually) so trades can be executed quickly
    3. The ability to split orders when the bid/ask quantities don't match
      * This can be treated similarly to a new order. The larger of the two orders
      gets split, and the new 'piece' becomes a new order.
      * Alternatively we can just change the quantity of the existing order
   If we just consider the first two requirements we need something like:
   ```c++
   using OrderPtr = std::shared_ptr<Order>
   using Orders = std::vector<OrderPtr>;
   std::map<Price, Orders>;
   ```
   So we get fast lookups for a particular price, and we can also quickly
   take the latest price since it'll be at the back of the vector.

   However, the approach above 'feels' naive/simplistic - you even alluded
   to this in our first conversation. So if we break it down:
   1. We can easily maintain a max and min for each list of orders as new orders come in
   2. The difficulty is keeping the list of orders sorted as new orders come in - 
      hence a sorted container like `std::map` being a good choice.
   3. Alternatively, we can just maintain the min and max and use a vector, but track
   its sorted state. This would have the problem of periodic spikes which would
   be pretty bad if the order list got long.

   It 'feels' like there's something I am missing here, but I can't quite put my finger
   on it. So I am keen to hear your thoughts here :)

3. Next we need to place the bids and asks into an order book

```cpp
class OrderBook {
    public:
    OrderBook();

    // Takes an order and returns
    // either the new order id or
    // the id of the order that's been
    // cancelled
    int Process(Event event);

    private:
    Bids bids_;
    Asks asks_;
};
```

4. If the bids and asks are appropriately sorted, the matching engine can be a simple function that matches the best bid to the best ask, optionally returning a trade:

```c++
struct Trade {
    int bid_id;
    int ask_id;
};

std::optional<Trade> Match(Bids& bids, Asks& asks);
```
