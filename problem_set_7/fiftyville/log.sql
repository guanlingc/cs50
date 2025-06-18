-- Keep a log of any SQL queries you execute as you solve the mystery.
-- July 28, on Humphrey Street 1015 am at Humphrey Street bakery
-- 10 tables

SELECT * FROM crime_scene_reports;
-- read crime scene report
-- theft occured 1015 am, 3 witness, all mentioned Bakery

SELECT * FROM interviews WHERE year = 2024 AND month = 7 AND day = 28;
-- Eugene says he recognize this person, in the morning before Emma Bakery, ATM at leggett street saw thief withdrawing money
-- Raymond says thief leaving bakery, called someone, less than a minute, earlies flight out on 29 July? asked the receiver to purchase air ticket
-- Ruth says within 10 min of theft, got into car at bakery parking lot

SELECT * FROM interviews WHERE name = 'Eugnene';
SELECT * FROM interviews WHERE name = 'Ruth';
SELECT * FROM interviews WHERE name = 'Raymond';
-- interview logs seem to be normal


SELECT * FROM atm_transactions
WHERE month = 7
AND day = 28
AND transaction_type = 'withdraw'
AND atm_location LIKE '%Leggett%';
-- account number for transaction on the day, but no timing
-- lots of record for july, filter to date and location
-- check cctv for cash withdrawal

SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE month = 7 AND day = 28
AND atm_transactions.account_number IN
    (
    SELECT account_number FROM atm_transactions
    WHERE month = 7
    AND day = 28
    AND transaction_type = 'withdraw'
    AND atm_location LIKE '%Leggett%'
    )
;
-- list of names who made withdrawal at leggett on 28 July
-- find the names of people associated with the withdrawal
-- +---------+
-- |  name   |
-- +---------+
-- | Bruce   |
-- | Diana   |
-- | Brooke  |
-- | Kenny   |
-- | Iman    |
-- | Luca    |
-- | Taylor  |
-- | Benista |
-- +---------+

SELECT name FROM phone_calls
JOIN people ON phone_calls.caller = people.phone_number
WHERE month = 7 AND day = 28
AND duration < 61;
-- phone call details of those who made phone calls on 28 july
-- use * to check if the retrieved table is correct
-- +---------+
-- |  name   |
-- +---------+
-- | Sofia   |
-- | Kelsey  |
-- | Bruce   |
-- | Kathryn |
-- | Kelsey  |
-- | Taylor  |
-- | Diana   |
-- | Carina  |
-- | Kenny   |
-- | Benista |
-- +---------+


SELECT name FROM phone_calls
JOIN people ON phone_calls.receiver = people.phone_number
WHERE month = 7 AND day = 28
AND duration < 61;

-- +------------+
-- |    name    |
-- +------------+
-- | Jack       |
-- | Larry      |
-- | Robin      |
-- | Luca       |
-- | Melissa    |
-- | James      |
-- | Philip     |
-- | Jacqueline |
-- | Doris      |
-- | Anna       |
-- +------------+

SELECT name FROM people
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE activity = 'exit'
AND people.license_plate IN
(
    SELECT license_plate FROM bakery_security_logs
    WHERE  year = 2024 AND month = 7 AND day = 28 AND hour = 10
    AND activity = 'exit'
    AND minute >= 15 AND minute <= 25
);
-- name of people with car plate numbers that left the carpark within 10 minutes of the theft
-- +---------+
-- |  name   |
-- +---------+
-- | Vanessa |
-- | Barry   |
-- | Iman    |
-- | Sofia   |
-- | Luca    |
-- | Diana   |
-- | Kelsey  |
-- | Bruce   |
-- +---------+

SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE month = 7 AND day = 28
AND atm_transactions.account_number IN
    (
    SELECT account_number FROM atm_transactions
    WHERE month = 7
    AND day = 28
    AND transaction_type = 'withdraw'
    AND atm_location LIKE '%Leggett%'
    )

INTERSECT

SELECT name FROM people
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE activity = 'exit'
AND people.license_plate IN
(
    SELECT license_plate FROM bakery_security_logs
    WHERE  year = 2024 AND month = 7 AND day = 28 AND hour = 10
    AND activity = 'exit'
    AND minute >= 15 AND minute <= 25
)

INTERSECT


SELECT name FROM phone_calls
JOIN people ON phone_calls.caller = people.phone_number
WHERE month = 7 AND day = 28
AND duration < 61;
-- list of suspects who made withdrawal from atm and made a call and left the carpark
-- suspects are Bruce and Diana

SELECT * FROM people
WHERE name in ('Diana', 'Bruce');
-- details of suspects
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+

SELECT * FROM flights
JOIN passengers ON flights.id = passengers.flight_id
WHERE passport_number IN
(
    SELECT passport_number FROM people
    WHERE name in ('Bruce')
);
-- where did bruce go
-- +----+-------------------+------------------------+------+-------+-----+------+--------+-----------+-----------------+------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute | flight_id | passport_number | seat |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+-----------+-----------------+------+
-- | 36 | 8                 | 4                      | 2024 | 7     | 29  | 8    | 20     | 36        | 5773159633      | 4A   |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+-----------+-----------------+------+

SELECT * FROM flights
JOIN passengers ON flights.id = passengers.flight_id
WHERE passport_number IN
(
    SELECT passport_number FROM people
    WHERE name in ('Diana')
);
-- where did diana go
-- +----+-------------------+------------------------+------+-------+-----+------+--------+-----------+-----------------+------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute | flight_id | passport_number | seat |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+-----------+-----------------+------+
-- | 18 | 8                 | 6                      | 2024 | 7     | 29  | 16   | 0      | 18        | 3592750733      | 4C   |
-- | 24 | 7                 | 8                      | 2024 | 7     | 30  | 16   | 27     | 24        | 3592750733      | 2C   |
-- | 54 | 8                 | 5                      | 2024 | 7     | 30  | 10   | 19     | 54        | 3592750733      | 6C   |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+-----------+-----------------+------+


SELECT name FROM phone_calls
JOIN people ON phone_calls.receiver = people.phone_number
WHERE month = 7 AND day = 28
AND duration < 61
AND caller = '(367) 555-5533';
-- who did bruce call