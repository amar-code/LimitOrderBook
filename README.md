How to build:

    mkdir -p build && cd build
    cmake ../
    make

    tested with:

    Apple LLVM version 10.0.1 (clang-1001.0.46.4)
    Target: x86_64-apple-darwin18.7.0
    Thread model: posix


Run

    ./OrderBook ../test/OrderValue.txt


Output:

--- TRADE --------------------------------------
100@12.2
---ORDER BOOK --------------------------------------
1008 buy 100 20.15
1006 buy 300 17.15
1004 buy 300 12.15

Total time Taken : 104990 per msg:8749 nanoseconds.

