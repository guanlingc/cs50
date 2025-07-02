CREATE TABLE trades (
                    id INTEGER NOT NULL,
                    transaction_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    symbol TEXT NOT NULL,
                    name TEXT NOT NULL,
                    shares INTEGER NOT NULL,
                    price NUMERIC NOT NULL,
                    transaction_type TEXT NOT NULL,
                    transacted TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
                    FOREIGN KEY(id) REFERENCES users(id)
                );
