import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # query database to get the relevant information
    user = session["user_id"]
    # get info from database
    rows = db.execute(
        "SELECT name, symbol, SUM(shares) FROM trades WHERE id = ? GROUP BY symbol", int(user)
    )  # returns a list of dicts (each dict contains stock information)

    # get price for each stock returned and add into list
    total_amount = 0
    for row in rows:
        # get current price
        current_stock_info = lookup(row['symbol'])
        # add it into dict
        row["current_price"] = current_stock_info['price']
        row["stock_value"] = current_stock_info['price'] * row['SUM(shares)']
        total_amount += row["stock_value"]
    # get account cash
    account_cash = db.execute(
        "SELECT cash FROM users WHERE id = ?", user
    )
    account_cash = account_cash[0]['cash']
    total_amount += account_cash

    return render_template("index.html", rows=rows, total_amount=total_amount, account_cash=account_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # if method = get ask for stock
    if request.method == "POST":
        # check inputs
        stock_to_buy = request.form.get("symbol")
        amount_to_buy = request.form.get("shares")
        stock_to_buy_info = lookup(stock_to_buy)  # returns a dict

        if not stock_to_buy:
            return apology("Please enter stock symbol")
        if not stock_to_buy_info:
            return apology("Stock code doesnt exit")
        if not amount_to_buy:
            return apology("Please input a valid value")

        # try to convert to float
        try:
            amount_to_buy_float = float(amount_to_buy)
            if amount_to_buy_float != int(amount_to_buy_float):
                return apology("Please input whole numbers")
            amount_to_buy = int(amount_to_buy_float)
        except ValueError:
            return apology("Please input whole numbers")

        if amount_to_buy <= 0:
            return apology("share number can't be negative number or zero!")

        # find account in db
        account_cash_db = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )
        # extract cash column as variable
        account_cash = account_cash_db[0]["cash"]

        # calculate cost of purchase
        cost_of_purchase = stock_to_buy_info["price"] * amount_to_buy

        # return error if not enought cash in account
        if cost_of_purchase > account_cash:
            return apology("Not enough credits, please top up")

        remaining_cash = account_cash - cost_of_purchase
        # update db new cash amount
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", remaining_cash, session["user_id"]
        )

        # update db on who bought what, how much and when
        db.execute(
            "INSERT INTO trades (id, name, symbol, shares, price, transaction_type) VALUES (?,?,?,?,?,?)",
            session["user_id"], stock_to_buy_info["name"], stock_to_buy_info["symbol"], amount_to_buy, stock_to_buy_info["price"], "BUY"
        )
        flash("Bought!")

        # if purchase succeed , return to home page
        return redirect("/")
    else:
        return render_template("buy_stock.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute(
        "SELECT name,symbol,shares,price,transaction_type,transacted FROM trades WHERE id = ? ORDER BY transacted DESC", session[
            "user_id"]
    )

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted

        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # to handle 2 scenarios (asking for quote and handling a quote)
    if request.method == "POST":
        stock_symbol = request.form.get("symbol")
        stock_info = lookup(stock_symbol)
        if not stock_info:
            return apology("Wrong Stock Symbol")
        return render_template("show_quote.html", stock_info=stock_info)
    else:
        return render_template("ask_for_quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        # information comes from register.html
        # 2 pcs of information passed here, username, pw and verify pw
        username = request.form.get("username")
        password = request.form.get("password")
        verify_password = request.form.get("confirmation")

        if password == "" or verify_password == "" or username == "":
            return apology("Please do not leave empty field")
        # check if both passwords match
        if password != verify_password:
            return apology("Passwords are not identical, please try again")

        # Query database for username
        existing_username = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )
        # checks if username is already in the database
        if len(existing_username) != 0:
            return apology("The username has been used, please try again")

        # If username doesnt exist in database and password is correct
        # generate password hash
        hash_password = generate_password_hash(password)
        # insert all of the relevant information into the db.
        db.execute(
            "INSERT INTO users (username,hash) VALUES (?,?)", request.form.get(
                "username"), hash_password
        )

        flash("Registration is successful")
        # username is registered and proceed to login
        return render_template("login.html")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # if method = get ask for stock
    if request.method == "POST":
        # check inputs
        stock_to_sell = request.form.get("symbol")
        amount_to_sell = int(request.form.get("shares"))
        stock_to_sell_info = lookup(stock_to_sell)  # returns a dict

        if not stock_to_sell:
            return apology("Please enter stock symbol")
        if not stock_to_sell_info:
            return apology("Stock code doesnt exist")
        if not amount_to_sell:
            return apology("Please input a valid value")

        # try to convert to float
        try:
            amount_to_sell_float = float(amount_to_sell)
            if amount_to_sell_float != int(amount_to_sell_float):
                return apology("Please input whole numbers")
            amount_to_sell = int(amount_to_sell_float)
        except ValueError:
            return apology("Please input whole numbers")

        if amount_to_sell <= 0:
            return apology("share number can't be negative number or zero!")

        # find account's stock
        current_stock = db.execute(
            "SELECT SUM(shares) FROM trades WHERE id = ?", session["user_id"]
        )

        # check for potential error
        if amount_to_sell > current_stock[0]['SUM(shares)']:
            return apology("You do not have enough shares")

        # find account in db
        account_cash_db = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )
        account_cash_db = account_cash_db[0]['cash']
        # calculate sale amount
        sale_amount = amount_to_sell * int(stock_to_sell_info['price'])

        # add sale to current account cash value
        account_cash_db += sale_amount
        amount_to_sell = -amount_to_sell
        # update db new cash amount
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", account_cash_db, session["user_id"]
        )

        # update db on who sold what, how much and when
        db.execute(
            "INSERT INTO trades (id, name, symbol, shares, price, transaction_type) VALUES (?,?,?,?,?,?)",
            session["user_id"], stock_to_sell_info["name"], stock_to_sell_info["symbol"], amount_to_sell, stock_to_sell_info["price"], "SELL"
        )
        flash("Sold!")

        # if purchase succeed , return to home page
        return redirect("/")
    else:
        rows = db.execute(
            "SELECT symbol, SUM(shares) FROM trades WHERE id = ? GROUP BY symbol", session["user_id"])

        return render_template("sell_stock.html", rows=rows)


@app.route("/topup_cash", methods=["GET", "POST"])
@login_required
def topup_cash():
    if request.method == "POST":
        if request.form.get("topup_amount") == request.form.get("topup_verify"):

            # get the input
            amount_to_top_up = int(request.form.get("topup_amount"))

            # find account in db
            account_cash_db = db.execute(
                "SELECT cash FROM users WHERE id = ?", session["user_id"]
            )
            account_cash_db = account_cash_db[0]['cash']

            # get new value
            account_cash_db += amount_to_top_up

            db.execute(
                "UPDATE users SET cash = ? WHERE id = ?", account_cash_db, session["user_id"]
            )

            # # update db on who sold what, how much and when
            # db.execute(
            #     "INSERT INTO trades (id, name, symbol, shares, price, transaction_type) VALUES (?,?,?,?,?,?)",
            #     session["user_id"], "NA", "NA", "NA", account_cash_db, "TOP UP"
            # )
            flash("Top Up Complete!")

            return redirect("/")
    else:
        return render_template("topup_cash.html")
