# Car Marketplace System
### OOP Assignment 02 — FAST National University, Karachi
**Name:** Izaan Aamir | **Roll No:** 25K-0059 | **Section:** BAI-2A
**Subject:** Object Oriented Programming | **Instructor:** Talha Shahid | **Spring 2026**

---

## Overview

This project is a CLI-based Car Marketplace System inspired by PakWheels. It was originally built in Assignment 01 using basic OOP concepts. This version (Assignment 02) extends it by applying advanced OOP concepts including Inheritance, Polymorphism, Abstraction, Operator Overloading, and Friend Functions.

The system allows:
- Sellers to post Car and Bike listings
- Admin to approve listings and view platform analytics
- Buyers to search, filter, bid, favourite, message sellers, and leave reviews

---

## File Structure

```
A2-25K-0059/
├── main.cpp          — main source file, all classes and logic
├── display.h    — abstract class 1
├── filter.h     — abstract class 2
├── user.h           — abstract class 3
└── README.md         — this file
```

---

## How to Compile and Run

Make sure all four files are in the same folder, then run:

```bash
g++ main.cpp -o marketplace
./marketplace
```

Or with Code Runner in VS Code — just open `main.cpp` and hit the run button. The `.h` files will be picked up automatically since they are in the same folder.

---

## Constants

| Constant | Value | Purpose |
|---|---|---|
| `MAX_LISTINGS` | 50 | Max listings in Marketplace and Seller |
| `MAX_FAVORITES` | 10 | Max favourites a Buyer can save |
| `MAX_MESSAGES` | 20 | Max messages in an inbox |
| `MAX_REVIEWS` | 20 | Max reviews a Seller can receive |
| `MAX_BIDS` | 10 | Max bids per Listing |
| `TAX_RATE` | 0.05 | Platform tax rate (5%) |
| `PLATFORM_FEE` | 5000 | Fixed platform fee in Rs. |

---

## Abstract Classes (Header Files)

### display — `display.h`
Forces any vehicle class to implement a standard display interface.

```cpp
class display {
public:
    virtual void displayDetails() const = 0;
    virtual string getCategory() const = 0;
    virtual ~display() {}
};
```

**Implemented by:** `Vehicle` (and therefore `Car` and `Bike`)
**Reason:** Car and Bike need to display different information. By enforcing `displayDetails()` as pure virtual, each subclass is guaranteed to provide its own display logic.

---

### filter — `filter.h`
Forces any vehicle class to support keyword-based searching.

```cpp
class filter {
public:
    virtual bool matchesFilter(string key) const = 0;
    virtual ~filter() {}
};
```

**Implemented by:** `Vehicle` (and therefore `Car` and `Bike`)
**Reason:** `Car` overrides `matchesFilter()` to also match `bodyStyle` (e.g. searching "SUV"), while the base `Vehicle` only matches brand and model. `Marketplace::filterByKeyword()` calls this polymorphically.

---

### user — `user.h`
Forces every user type to implement a panel label and a profile view.

```cpp
class user {
public:
    virtual void showPanel() = 0;
    virtual void showProfile() const = 0;
    virtual ~user() {}
};
```

**Implemented by:** `User` (and therefore `Buyer`, `Seller`, `Admin`)
**Reason:** All three roles share the same interface but show completely different information. Used in `main()` with a `User*` array to demonstrate runtime polymorphism.

---

## Classes

### Review
Stores a star rating and comment left by a buyer on a seller.

| Member | Type | Description |
|---|---|---|
| `raterId` | `int` | ID of the buyer leaving the review |
| `stars` | `int` | Rating out of 5 |
| `comment` | `string` | Written feedback |

**Key methods:**
- `operator==` — checks if two reviews are from the same buyer (prevents duplicates)
- `display()` — prints the review
- `friend calcAvgRating()` — friend function that accesses `stars` to compute average

---

### Message
Stores a message sent from a buyer to a seller's inbox.

| Member | Type | Description |
|---|---|---|
| `senderId` | `int` | ID of the sender |
| `text` | `string` | Message content |
| `read` | `bool` | Whether the message has been read |

**Key methods:**
- `setRead()` — marks message as read
- `operator==` — checks if an identical message already exists
- `display()` — prints sender ID and message text
- `friend printInbox()` — friend function that prints a full Message array

---

### Vehicle
Base class for all vehicle types. Implements `display` and `IFilterable`. Stores type-specific data in `extra1` and `extra2` fields to avoid needing pointers.

| Member | Type | Description |
|---|---|---|
| `brand` | `string` | Vehicle brand |
| `model` | `string` | Vehicle model |
| `year` | `int` | Manufacturing year |
| `mileage` | `int` | Kilometres driven |
| `price` | `float` | Listed price in Rs. |
| `type` | `string` | "Car", "Bike", or "Vehicle" |
| `extra1` | `string` | Body style (Car) or ride style (Bike) |
| `extra2` | `string` | Door count (Car) or CC (Bike) |

**Key methods:**
- `operator==` — compares brand, model, year to detect duplicate listings
- `operator+` — adds two vehicle prices together (bundle display)
- `matchesFilter(key)` — returns true if brand, model, or extra1 matches key
- `displayDetails()` — prints vehicle info, then Car or Bike specific fields based on `type`
- `friend class Marketplace` — lets Marketplace access private `price` directly in `filterByPrice()`

---

### Car *(derived from Vehicle)*
Represents a car listing. Sets `type` to `"Car"`, stores body style in `extra1` and door count in `extra2`.

```cpp
Car c("Toyota", "Corolla", 2020, 45000, 3500000.0, "Sedan", "4");
```

**Overrides:** `displayDetails()`, `getCategory()`
**matchesFilter** also checks `extra1` (body style) so buyers can search by "SUV" etc.

---

### Bike *(derived from Vehicle)*
Represents a bike listing. Sets `type` to `"Bike"`, stores ride style in `extra1` and CC in `extra2`.

```cpp
Bike b("Honda", "CB150F", 2022, 12000, 320000.0, "Sport", "150");
```

**Overrides:** `displayDetails()`, `getCategory()`

---

### Bid
Represents a single bid placed by a buyer on a listing.

| Member | Type | Description |
|---|---|---|
| `bidderId` | `int` | ID of the buyer placing the bid |
| `amount` | `float` | Bid amount in Rs. |

**Key methods:**
- `operator==` — checks if same buyer already bid (prevents duplicate bids)
- `operator>` — compares bid amounts, used to find the highest bid in `showBids()`
- `display()` — prints bidder ID and amount

---

### Listing
Represents a vehicle listing posted by a seller. Stores the Vehicle by value (same as A1). Also holds an array of Bids.

| Member | Type | Description |
|---|---|---|
| `listingId` | `int` | Unique listing ID |
| `vehicle` | `Vehicle` | The vehicle being listed (by value) |
| `approved` | `bool` | Whether admin has approved it |
| `sellerId` | `int` | ID of the seller who posted it |
| `bids[]` | `Bid[MAX_BIDS]` | All bids placed on this listing |
| `bidCount` | `int` | Number of bids placed |
| `totalListings` | `static int` | Total listings ever created |

**Key methods:**
- `approve()` — sets approved to true
- `placeBid(buyerId, amt)` — adds a bid, blocks duplicates using `Bid::operator==`
- `showBids()` — prints all bids and finds highest using `Bid::operator>`
- `getTotalListings()` — static method returning total listing count
- `display()` — prints listing ID, vehicle details, and approval status

---

### Favorite
Wraps a Listing object. Used by Buyer to save listings.

| Member | Type | Description |
|---|---|---|
| `listing` | `Listing` | A copy of the saved listing |

**Key methods:**
- `display()` — calls `listing.display()`

---

### User *(abstract, implements user)*
Abstract base class for all user types. Holds common fields and forces subclasses to implement `showPanel()` and `showProfile()`.

| Member | Type | Description |
|---|---|---|
| `userId` | `int` (protected) | User ID |
| `name` | `string` (protected) | Full name |
| `email` | `string` (protected) | Email address |

---

### Buyer *(derived from User)*
Represents a buyer. Can save favourites, receive messages, and place bids.

| Member | Type | Description |
|---|---|---|
| `favorites[]` | `Favorite[MAX_FAVORITES]` | Saved listings |
| `favCount` | `int` | Number of favourites |
| `inbox[]` | `Message[MAX_MESSAGES]` | Received messages |
| `msgCount` | `int` | Number of messages |

**Key methods:**
- `addFavorite(l)` — saves a listing to favourites
- `viewFavorites()` — displays all saved listings
- `sendMessage(sid, text, inbox, count)` — pushes a Message into the seller's inbox array
- `viewInbox()` — calls `printInbox()` friend function
- `showPanel()` / `showProfile()` — override from user

---

### Seller *(derived from User)*
Represents a seller. Owns listings and receives messages and reviews.

| Member | Type | Description |
|---|---|---|
| `myListings[]` | `Listing*[MAX_LISTINGS]` | Pointers to seller's own listings |
| `listingCount` | `int` | Number of listings |
| `inbox[]` | `Message[MAX_MESSAGES]` | Received messages |
| `msgCount` | `int` | Number of messages |
| `reviews[]` | `Review[MAX_REVIEWS]` | Reviews received from buyers |
| `reviewCount` | `int` | Number of reviews |

**Key methods:**
- `addListing(l*)` — adds a pointer to a listing
- `viewMessages()` — calls `printInbox()` friend function
- `addReview(r)` — adds a review, blocks duplicates using `Review::operator==`
- `viewReviews()` — displays all reviews and calls `calcAvgRating()`
- `showPanel()` / `showProfile()` — override from user

---

### Admin *(derived from User)*
Represents the admin. Can approve listings and trigger platform analytics.

**Key methods:**
- `approveListing(l)` — calls `l.approve()`
- `showPanel()` / `showProfile()` — override from user

---

### Marketplace
Central system class. Stores all listings by value in a fixed array (same as A1). Has multiple filter methods.

| Member | Type | Description |
|---|---|---|
| `listings[]` | `Listing[MAX_LISTINGS]` | All listings in the system |
| `count` | `int` | Number of listings |

**Key methods:**
- `addListing(l)` — adds a listing by value
- `getListingById(id)` — returns a pointer to the matching listing or NULL
- `viewApprovedListings()` — displays only approved listings
- `filterByType(t)` — filters by "Car" or "Bike" using `getCategory()`
- `filterByKeyword(key)` — filters using polymorphic `matchesFilter(key)`
- `filterByPrice(minP, maxP)` — filters using direct price access (friend class)
- `showTotalListings()` — prints the static total listing count
- `friend showPlatformSummary()` — friend function accessing private `listings[]` and `count`

---

## Inheritance Hierarchy

```
user (abstract)
    └── User (abstract)
            ├── Buyer
            ├── Seller
            └── Admin

display (abstract) ──┐
IFilterable  (abstract) ──┴── Vehicle
                                ├── Car
                                └── Bike
```

### All 6 Inheritance Relationships

| # | Relationship | Type | Reason |
|---|---|---|---|
| 1 | user → User | Abstract implements | Forces showPanel() and showProfile() on all user types |
| 2 | display → Vehicle | Abstract implements | Forces standard display interface on all vehicle types |
| 3 | IFilterable → Vehicle | Abstract implements | Forces keyword filter on all vehicle types |
| 4 | Vehicle → Car | Concrete inheritance | Car adds body style and door count to base Vehicle |
| 5 | Vehicle → Bike | Concrete inheritance | Bike adds ride style and CC to base Vehicle |
| 6 | User → Buyer/Seller/Admin | Concrete inheritance | Three roles share name/email but have different capabilities |

---

## Polymorphism

### Runtime Polymorphism (Function Overriding)

`displayDetails()` is overridden in `Car` and `Bike`. When called via a `Vehicle` reference inside `Listing::display()`, the correct version runs at runtime:

```cpp
// Inside Listing::display()
vehicle.displayDetails();  // calls Car or Bike version depending on type
```

`showProfile()` and `showPanel()` are overridden in all three user types. Demonstrated at startup via a `User*` array:

```cpp
User* users[3];
users[0] = &admin;
users[1] = &seller;
users[2] = &buyer;

for (int i = 0; i < 3; i++)
    users[i]->showProfile();  // dispatches to correct subclass at runtime
```

`matchesFilter()` is overridden in `Car` to also match `extra1` (body style):

```cpp
// Called polymorphically in filterByKeyword()
listings[i].getVehicle().matchesFilter(key);
```

### Compile-time Polymorphism (Function Overloading)

All major classes have both a default constructor and a parameterised constructor — `Vehicle`, `Car`, `Bike`, `Bid`, `Review`, `Message`, `Listing`, `Buyer`, `Seller`, `Admin`.

---

## Operator Overloading

| Class | Operator | Return | Reason |
|---|---|---|---|
| `Vehicle` | `==` | `bool` | Compare brand + model + year to detect duplicate listings |
| `Vehicle` | `+` | `float` | Add prices of two vehicles for bundle display |
| `Bid` | `==` | `bool` | Check if same buyer already placed a bid on a listing |
| `Bid` | `>` | `bool` | Compare bid amounts to find the highest bid |
| `Review` | `==` | `bool` | Check if same buyer already reviewed this seller |
| `Message` | `==` | `bool` | Check if an identical message already exists |

**Example — duplicate bid prevention:**
```cpp
Bid b(buyerId, amt);
for (int i = 0; i < bidCount; i++) {
    if (bids[i] == b) {
        cout << "You already placed a bid.\n";
        return;
    }
}
```

**Example — highest bid:**
```cpp
Bid highest = bids[0];
for (int i = 1; i < bidCount; i++)
    if (bids[i] > highest)
        highest = bids[i];
```

---

## Friend Functions

| Friend | Declared in | Accesses | Reason |
|---|---|---|---|
| `calcAvgRating(arr, count)` | `Review` | private `stars` | Computes average star rating without needing a public getter on every element |
| `printInbox(arr, count)` | `Message` | private `text`, `read` | Prints a full inbox array from outside the class cleanly |
| `showPlatformSummary(market)` | `Marketplace` | private `listings[]`, `count` | Admin analytics function needs direct array access without exposing it publicly |
| `friend class Marketplace` | `Vehicle` | private `price` | `filterByPrice()` compares price directly instead of going through a getter each time |

**Example — calcAvgRating:**
```cpp
float calcAvgRating(Review arr[], int count) {
    float total = 0;
    for (int i = 0; i < count; i++)
        total += arr[i].stars;  // direct access to private field
    return total / count;
}
```

---

## Composition and Aggregation

| Relationship | Type | Reason |
|---|---|---|
| `Listing` has `Vehicle` | Composition | Vehicle is stored by value inside Listing, destroyed with it |
| `Listing` has `Bid[]` | Composition | Bids live inside Listing, no existence outside |
| `Favorite` has `Listing` | Composition | Listing copy lives inside Favorite |
| `Buyer` has `Favorite[]` | Composition | Favourites array lives inside Buyer |
| `Buyer` has `Message[]` | Composition | Inbox lives inside Buyer |
| `Seller` has `Review[]` | Composition | Reviews live inside Seller |
| `Marketplace` has `Listing[]` | Composition | All listings stored by value in Marketplace array |
| `Seller` has `Listing*[]` | Aggregation | Seller holds pointers to listings that exist in Marketplace |

---

## Static Members

| Member | Class | Description |
|---|---|---|
| `totalListings` | `Listing` | Counts every Listing ever constructed. Incremented in parameterised constructor. Accessed via `Listing::getTotalListings()` |

---

## New Features vs Assignment 01

| Feature | A1 | A2 |
|---|---|---|
| Vehicle types | Single `Vehicle` class | `Car` and `Bike` derived classes |
| Search | Not present | Filter by type, brand/model keyword, price range |
| Bidding | Not present | Buyers can bid; duplicate bids blocked; highest bid tracked |
| Seller reviews | Not present | Buyers rate sellers 1-5 stars; average via friend function |
| Platform summary | Not present | Friend function shows total listings and their combined value |
| Polymorphic profiles | Not present | `User*` array calls `showProfile()` for all roles at startup |

---

## Menu Structure

```
===== CAR MARKETPLACE SYSTEM =====
1. Admin
2. Seller
3. Buyer
0. Exit

--- ADMIN PANEL ---
1. Approve Listing
2. View Approved Listings
3. View Bids on Listing
4. Platform Summary
0. Back

--- SELLER PANEL ---
1. Add Car Listing
2. Add Bike Listing
3. View Messages
4. View My Reviews
0. Back

--- BUYER PANEL ---
1. View Approved Listings
2. Filter by Type (Car/Bike)
3. Filter by Brand or Model
4. Filter by Price Range
5. Add to Favorites
6. View Favorites
7. Place a Bid
8. Message Seller
9. Leave a Review
0. Back
```

---

## Sample Usage

```
Select Role: 2  (Seller)
Choice: 1       (Add Car Listing)

Enter Listing ID: 101
Enter Brand: Toyota
Enter Model: Corolla
Enter Year: 2020
Enter Mileage: 45000
Enter Price: 3500000
Enter Body Style: Sedan
Enter Doors: 4
→ Car listing added (Pending Approval).

Select Role: 1  (Admin)
Choice: 1       (Approve Listing)
Enter Listing ID: 101
→ Listing approved.

Select Role: 3  (Buyer)
Choice: 7       (Place a Bid)
Enter Listing ID: 101
Enter bid amount: 3400000
→ Bid placed: Rs.3400000
```

---

## OOP Concepts Checklist

| Concept | Implemented | Where |
|---|---|---|
| Classes | Yes | All 16 classes |
| Default constructors | Yes | All classes |
| Parameterised constructors | Yes | All classes |
| Copy constructor | Yes | `Vehicle` |
| Static member | Yes | `Listing::totalListings` |
| Const functions | Yes | All getter methods, display methods |
| Abstract classes | Yes | `display`, `IFilterable`, `user` |
| Pure virtual functions | Yes | 6 pure virtual methods across 3 abstract classes |
| Inheritance | Yes | 6 relationships |
| Function overriding | Yes | `displayDetails()`, `matchesFilter()`, `showPanel()`, `showProfile()`, `getCategory()` |
| Function overloading | Yes | Multiple constructors in all classes |
| Operator overloading | Yes | 6 operators across 4 classes |
| Friend functions | Yes | 3 friend functions + 1 friend class |
| Composition | Yes | 7 relationships |
| Aggregation | Yes | `Seller::myListings[]` |
| Array of objects | Yes | `Marketplace::listings[]`, `Listing::bids[]`, `Buyer::favorites[]` |
| Pointer to base class | Yes | `User* users[]` in main(), `Listing*` in Seller |