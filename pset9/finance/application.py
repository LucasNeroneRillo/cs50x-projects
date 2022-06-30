import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from termcolor import cprint

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get all the stocks the user have
    stocks = db.execute("SELECT * FROM stocks WHERE user_id = ?", session["user_id"])

    # Each stock is a dict that contains the symbol and number of shares of each stock
    # Add to each dict: current stock's price, company's name, and total price of stock's shares
    stocks_value = 0
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["price"] = quote["price"]
        stock["name"] = quote["name"]
        stock["total"] = stock["price"] * stock["shares"]
        stocks_value += stock["total"]

    # Get user's cash and total assets (cash + stocks' value)
    rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    cash = rows[0]["cash"]
    total = cash + stocks_value

    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User posted form to buy shares
    if request.method == 'POST':

        # Get quote, ensuring symbol provided is valid
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("invalid symbol", 400)

        # Ensure number of shares is a positive integer
        if not request.form.get("shares").isdigit():
            return apology("number of shares must be a positive integer", 400)
        shares = int(request.form.get("shares"))

        # Ensure user has enough money to buy the shares
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        account_cash = rows[0]["cash"]
        total_price = quote["price"] * shares
        if total_price > account_cash:
            return apology("not enough money to buy shares", 400)

        user_id = session["user_id"]
        symbol = quote["symbol"]

        # Add purchase to database's transactions table
        db.execute("""INSERT INTO transactions (user_id, symbol, shares, price)
                   VALUES (?, ?, ?, ?)""",
                   user_id, symbol, shares, quote["price"])

        # Update database's stocks table
        rows = db.execute("SELECT * FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol)
        if rows:
            # User already had stocks from that company
            previous_shares = rows[0]["shares"]
            db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ?",
                       (previous_shares + shares), user_id, symbol)
        else:
            # User did not have stocks from that company
            db.execute("INSERT INTO stocks (user_id, symbol, shares) VALUES (?, ?, ?)",
                       user_id, symbol, shares)

        # Update user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   (account_cash - total_price), session["user_id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", transactions=transactions)


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
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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

    if request.method == 'POST':

        quote = lookup(request.form.get("symbol"))

        # Check if symbol provided is valid
        if not quote:
            return apology("invalid symbol", 400)

        # Return quote's info to user
        return render_template("quoted.html", quote=quote)

    if request.method == 'GET':
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == 'POST':
        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure username contains 6-40 chars
        """"
        if len(username) < 6 or len(username) > 40:
            return apology("username must contain 6-40 characters", 400)
        """

        # Ensure password was submitted
        if not password:
            return apology("must provide password", 400)

        # Ensure both passwords match
        elif not password == request.form.get("confirmation"):
            return apology("passwords must match", 400)

        # Ensure password contains 8-200 chars, having at least one digit,
        # one uppercase letter, one lowercase, and one symbol
        """"
        if (    len(password) < 8 or len(password) > 200
                or not re.search(r"\d", password)
                or not re.search(r"[A-Z]", password)
                or not re.search(r"[a-z]", password)
                or not re.search(r"\W", password)):
            return apology("invalid password", 400)
        """

        # Check in database if username already exists
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if rows:
            return apology("username already exists", 400)

        # Hash password
        password_hash = generate_password_hash(password)

        # Register user
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, password_hash)

        # Log user in
        user_id = db.execute("SELECT id FROM users WHERE username = ?", username)[0]["id"]
        session.clear()
        session["user_id"] = user_id

        # Redirect user to home page
        return redirect("/")

    # Display regitration form
    if request.method == 'GET':
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure user own stocks from the symbol typed
        symbol = request.form.get("symbol")
        stocks = db.execute("SELECT * FROM stocks WHERE symbol = ?", symbol)
        if not stocks:
            return apology("you do not own stocks from that company", 400)

        # Ensure number of shares user is trying to sell is a positive integer
        if not request.form.get("shares").isdigit():
            return apology("number of shares to sell must be a positive integer", 400)
        shares_to_sell = int(request.form.get("shares"))

        # Ensure user has enough shares to sell
        if shares_to_sell > stocks[0]["shares"]:
            return apology("you do not have that many shares to sell", 400)

        # Sell the shares, updating all three database tables
        user_id = session["user_id"]
        quote = lookup(symbol)
        previous_cash = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]["cash"]
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   (previous_cash + shares_to_sell * quote["price"]), user_id)
        db.execute("""INSERT INTO transactions (user_id, symbol, shares, price)
                   VALUES (?, ?, ?, ?)""",
                   user_id, symbol, -shares_to_sell, quote["price"])
        # If user is selling all stocks, remove the company from user's stock portfolio
        if shares_to_sell == stocks[0]["shares"]:
            db.execute("DELETE FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol)
        else:
            db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ?",
                       (stocks[0]["shares"] - shares_to_sell), user_id, symbol)

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get all the stocks the user owns
        stocks = db.execute("SELECT * FROM stocks WHERE user_id = ?", session["user_id"])

        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
