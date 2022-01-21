Looking at results.json shows a clear trend with interactions/sec and N. As the number of bodies/threads increases, the interactions/sec linearly increases. Comparing the values from the Day 20 assignment, N = 2000 is about the break-even point where N < 2000 has worse performance and N > 2000 has better performance. We would expect a relationship like this because large amounts of data benefits from parallelism.

My solution doesn't require locks/mutexes because of the way my algorithm is designed. The potential race condition for a program like this would be a body's position when another body is calculating it's forceVector as force depends on distance, but I make sure to sync threads before updating position. I experimented with having the cout labelled by "// Print body stat at start of step" be within the thread and with a mutex locking cout, but it would not always print the bodies in numerical order. If I was graphing this information instead of printing, then the order wouldn't matter and we could put it within the thread, but I just use it as a sanity check to make sure the data is correct by looking at it, so I value order.