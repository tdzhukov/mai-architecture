# Performance testing

## Without Redis
```
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.33ms    1.89ms  41.62ms   97.20%
    Req/Sec     0.89k   116.97     1.30k    86.59%
  Latency Distribution
     50%    1.10ms
     75%    1.41ms
     90%    2.05ms
     99%    8.03ms
  89558 requests in 10.08s, 25.33MB read
Requests/sec:   8884.77
Transfer/sec:      2.51MB
```

## With Redis
```
Running 10s test @ http://localhost:8080
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.00ms    1.11ms  29.61ms   97.86%
    Req/Sec     1.10k   132.62     1.27k    89.41%
  Latency Distribution
     50%  847.00us
     75%    1.05ms
     90%    1.31ms
     99%    4.48ms
  110696 requests in 10.10s, 31.75MB read
Requests/sec:  10958.05
Transfer/sec:      3.14MB
```