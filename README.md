1. list of online users.
    key
        mt5:users_online
    example
        {[
            { "login": 123123, "group": "demo", "ip": "127.0.0.1"},
            { "login": 123124, "group": "demo", "ip": "127.0.0.1"}
        ]}

2. positions per user for online users
    key
        mt5:positions:<login>
    example
        {[
            {"ticket": 1313, "volume": 0.1, "price_open": 1.12121, "sl":1.2323, "tp": 1.111, "time_open": "2000.02.02 23:22"},
            {"ticket": 1314, "volume": 0.1, "price_open": 1.12121, "sl":1.2323, "tp": 1.111, "time_open": "2000.02.02 23:21"}
        ]}

3. user margin/equity/profit info for online users
    key
        mt5:info:<login>
    example
        {"profit": 12, "equity": 333333, "margin": 1211.12, "margin_free": 121.11}
    
