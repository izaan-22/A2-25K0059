#include <iostream>
#include <string>
#include "display.h"
#include "filter.h"
#include "user.h"
using namespace std;

const int MAX_LISTINGS = 50;
const int MAX_FAVORITES = 10;
const int MAX_MESSAGES = 20;
const int MAX_REVIEWS = 30;
const int MAX_BIDS = 10;
const float TAX_RATE = 0.05;
const int PLATFORM_FEE = 5000;

class Review {
private:
    int raterId;
    int stars;
    string comment;

public:
    Review() {
        raterId = 0;
        stars = 0;
        comment = "";
    }

    Review(int rid, int s, string c) {
        raterId = rid;
        stars = s;
        comment = c;
    }

    bool operator==(const Review& r) const {
        return raterId == r.raterId;
    }

    int getStars() const {
        return stars;
    }

    void display() const {
        cout << "Rater ID: " << raterId << " | Stars: " << stars << "/5 | " << comment << endl;
    }

    friend float calcAvgRating(Review arr[], int count);
};

float calcAvgRating(Review arr[], int count) {
    if (count == 0) return 0;
    float total = 0;
    for (int i = 0; i < count; i++)
        total += arr[i].stars;
    return total / count;
}

class Message {
private:
    int senderId;
    string text;
    bool isRead;

public:
    Message() {
        senderId = 0;
        text = "";
        isRead = false;
    }

    Message(int sid, string t) {
        senderId = sid;
        text = t;
        isRead = false;
    }

    void markRead() {
        isRead = true;
    }

    bool operator==(const Message& m) const {
        return senderId == m.senderId && text == m.text;
    }

    void display() const {
        cout << "From ID: " << senderId << " | " << text;
        if (isRead)
            cout << " [Read]";
        cout << endl;
    }

    friend void printInbox(Message arr[], int count);
};

void printInbox(Message arr[], int count) {
    if (count == 0) {
        cout << "No messages.\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". ";
        arr[i].display();
    }
}

class Vehicle : public display, public filter {
private:
    string brand;
    string model;
    int year;
    int mileage;
    float price;

public:
    Vehicle() {
        brand = "";
        model = "";
        year = 0;
        mileage = 0;
        price = 0;
    }

    Vehicle(string b, string m, int y, int mile, float p) {
        brand = b;
        model = m;
        year = y;
        mileage = mile;
        price = p;
    }

    Vehicle(const Vehicle& v) {
        brand = v.brand;
        model = v.model;
        year = v.year;
        mileage = v.mileage;
        price = v.price;
    }

    float getPrice() const { 
        return price; 
    }
    string getBrand() const { 
        return brand; 
    }
    string getModel() const { 
        return model; 
    }
    int getYear() const { 
        return year; 
    }
    int getMileage() const { 
        return mileage; 
    }

    bool operator==(const Vehicle& v) const {
        return brand == v.brand && model == v.model && year == v.year;
    }

    float operator+(const Vehicle& v) const {
        return price + v.price;
    }

    bool matchesFilter(string key) const override {
        return brand == key || model == key;
    }

    void displayDetails() const override {
        cout << brand << " " << model << " (" << year << ") "
             << mileage << "km  Rs." << price << endl;
    }

    string getType() const override {
        return "Vehicle";
    }

    friend class Marketplace;
};

class Car : public Vehicle {
private:
    int doors;
    string bodyStyle;

public:
    Car() : Vehicle() {
        doors = 4;
        bodyStyle = "";
    }

    Car(string b, string m, int y, int mile, float p, int d, string style)
        : Vehicle(b, m, y, mile, p) {
        doors = d;
        bodyStyle = style;
    }

    void displayDetails() const override {
        Vehicle::displayDetails();
        cout << "  Body: " << bodyStyle << " | Doors: " << doors << endl;
    }

    string getType() const override {
        return "Car";
    }

    bool matchesFilter(string key) const override {
        return Vehicle::matchesFilter(key) || bodyStyle == key;
    }
};

class Bike : public Vehicle {
private:
    int cc;
    string bikeStyle;

public:
    Bike() : Vehicle() {
        cc = 0;
        bikeStyle = "";
    }

    Bike(string b, string m, int y, int mile, float p, int c, string style)
        : Vehicle(b, m, y, mile, p) {
        cc = c;
        bikeStyle = style;
    }

    void displayDetails() const override {
        Vehicle::displayDetails();
        cout << "  Style: " << bikeStyle << " | CC: " << cc << endl;
    }

    string getType() const override {
        return "Bike";
    }
};

class Bid {
private:
    int bidderId;
    float amount;

public:
    Bid() {
        bidderId = 0;
        amount = 0;
    }

    Bid(int id, float amt) {
        bidderId = id;
        amount = amt;
    }

    bool operator>(const Bid& b) const {
        return amount > b.amount;
    }

    bool operator==(const Bid& b) const {
        return bidderId == b.bidderId;
    }

    float getAmount() const { return amount; }
    int getBidderId() const { return bidderId; }

    void display() const {
        cout << "Bidder ID: " << bidderId << " | Amount: Rs." << amount << endl;
    }
};

class Listing {
private:
    static int totalListings;
    int listingId;
    Vehicle* vehicle;
    bool approved;
    int sellerId;
    Bid bids[MAX_BIDS];
    int bidCount;

public:
    Listing() {
        listingId = 0;
        vehicle = nullptr;
        approved = false;
        sellerId = 0;
        bidCount = 0;
    }

    Listing(int id, Vehicle* v, int sid) {
        listingId = id;
        vehicle = v;
        sellerId = sid;
        approved = false;
        bidCount = 0;
        totalListings++;
    }

    void approve() {
        approved = true;
    }

    bool isApproved() const { return approved; }
    int getId() const { return listingId; }
    int getSellerId() const { return sellerId; }
    Vehicle* getVehicle() const { return vehicle; }

    void placeBid(int buyerId, float amt) {
        if (!approved) {
            cout << "Listing not approved yet.\n";
            return;
        }
        if (bidCount >= MAX_BIDS) {
            cout << "Bid limit reached.\n";
            return;
        }
        Bid newBid(buyerId, amt);
        for (int i = 0; i < bidCount; i++) {
            if (bids[i] == newBid) {
                cout << "You already placed a bid.\n";
                return;
            }
        }
        bids[bidCount++] = newBid;
        cout << "Bid placed: Rs." << amt << endl;
    }

    void showBids() const {
        if (bidCount == 0) {
            cout << "No bids yet.\n";
            return;
        }
        for (int i = 0; i < bidCount; i++)
            bids[i].display();
    }

    Bid getHighestBid() const {
        Bid highest = bids[0];
        for (int i = 1; i < bidCount; i++)
            if (bids[i] > highest)
                highest = bids[i];
        return highest;
    }

    static int getTotalListings() {
        return totalListings;
    }

    void display() const {
        cout << "\nListing ID: " << listingId << " | Seller ID: " << sellerId << endl;
        if (vehicle)
            vehicle->displayDetails();
        cout << "Status: " << (approved ? "Approved" : "Pending") << endl;
    }
};

int Listing::totalListings = 0;

class User : public user {
protected:
    int userId;
    string name;
    string email;

public:
    User() {
        userId = 0;
        name = "";
        email = "";
    }

    User(int id, string n, string e) {
        userId = id;
        name = n;
        email = e;
    }

    int getId() const { 
        return userId; 
    }

    virtual void showPanel() = 0;
    virtual void showProfile() const = 0;

    virtual ~User() {}
};

class Buyer : public User {
private:
    Listing* favorites[MAX_FAVORITES];
    int favCount;
    Message inbox[MAX_MESSAGES];
    int msgCount;
    Review givenReviews[MAX_REVIEWS];
    int reviewCount;

public:
    Buyer() : User() {
        favCount = 0;
        msgCount = 0;
        reviewCount = 0;
    }

    Buyer(int id, string n, string e) : User(id, n, e) {
        favCount = 0;
        msgCount = 0;
        reviewCount = 0;
    }

    void addFavorite(Listing* l) {
        for (int i = 0; i < favCount; i++) {
            if (favorites[i]->getId() == l->getId()) {
                cout << "Already in favorites.\n";
                return;
            }
        }
        if (favCount < MAX_FAVORITES) {
            favorites[favCount++] = l;
            cout << "Added to favorites.\n";
        }
    }

    void viewFavorites() const {
        if (favCount == 0) {
            cout << "No favorites saved.\n";
            return;
        }
        for (int i = 0; i < favCount; i++)
            favorites[i]->display();
    }

    void receiveMessage(Message m) {
        if (msgCount < MAX_MESSAGES)
            inbox[msgCount++] = m;
    }

    void viewInbox() {
        printInbox(inbox, msgCount);
    }

    void leaveReview(int raterId, int stars, string comment, Review sellerReviews[], int& count) {
        Review r(raterId, stars, comment);
        for (int i = 0; i < count; i++) {
            if (sellerReviews[i] == r) {
                cout << "You already reviewed this seller.\n";
                return;
            }
        }
        if (count < MAX_REVIEWS) {
            sellerReviews[count++] = r;
            cout << "Review submitted.\n";
        }
    }

    void showPanel() override {
        cout << "Buyer Panel\n";
    }

    void showProfile() const override {
        cout << "Buyer | ID: " << userId << " | Name: " << name << " | Email: " << email << endl;
    }
};

class Seller : public User {
private:
    Listing* myListings[MAX_LISTINGS];
    int listingCount;
    Message inbox[MAX_MESSAGES];
    int msgCount;
    Review reviews[MAX_REVIEWS];
    int reviewCount;

public:
    Seller() : User() {
        listingCount = 0;
        msgCount = 0;
        reviewCount = 0;
    }

    Seller(int id, string n, string e) : User(id, n, e) {
        listingCount = 0;
        msgCount = 0;
        reviewCount = 0;
    }

    void addListing(Listing* l) {
        if (listingCount < MAX_LISTINGS)
            myListings[listingCount++] = l;
    }

    void viewMyListings() const {
        for (int i = 0; i < listingCount; i++)
            myListings[i]->display();
    }

    void viewInbox() {
        printInbox(inbox, msgCount);
    }

    void showRating() const {
        cout << "Avg Rating: " << calcAvgRating(const_cast<Review*>(reviews), reviewCount) << "/5\n";
    }

    void showReviews() const {
        for (int i = 0; i < reviewCount; i++)
            reviews[i].display();
    }

    Message* getInbox() { return inbox; }
    int& getMsgCount() { return msgCount; }
    Review* getReviews() { return reviews; }
    int& getReviewCount() { return reviewCount; }

    void showPanel() override {
        cout << "Seller Panel\n";
    }

    void showProfile() const override {
        cout << "Seller | ID: " << userId << " | Name: " << name << " | Email: " << email << endl;
    }
};

class Admin : public User {
public:
    Admin() : User(1, "Admin", "admin@market.com") {}

    Admin(int id, string n, string e) : User(id, n, e) {}

    void approveListing(Listing& l) {
        l.approve();
        cout << "Listing " << l.getId() << " approved.\n";
    }

    void showPanel() override {
        cout << "Admin Panel\n";
    }

    void showProfile() const override {
        cout << "Admin | ID: " << userId << " | Name: " << name << endl;
    }
};

class Marketplace {
private:
    Listing* listings[MAX_LISTINGS];
    int count;

public:
    Marketplace() {
        count = 0;
    }

    void addListing(Listing* l) {
        if (count < MAX_LISTINGS)
            listings[count++] = l;
    }

    Listing* getById(int id) {
        for (int i = 0; i < count; i++)
            if (listings[i]->getId() == id)
                return listings[i];
        return nullptr;
    }

    void viewApproved() const {
        bool found = false;
        for (int i = 0; i < count; i++) {
            if (listings[i]->isApproved()) {
                listings[i]->display();
                found = true;
            }
        }
        if (!found)
            cout << "No approved listings.\n";
    }

    void filterByType(string t) const {
        bool found = false;
        for (int i = 0; i < count; i++) {
            if (listings[i]->isApproved() && listings[i]->getVehicle() != nullptr) {
                if (listings[i]->getVehicle()->getType() == t) {
                    listings[i]->display();
                    found = true;
                }
            }
        }
        if (!found)
            cout << "No listings found for type: " << t << endl;
    }

    void filterByKeyword(string key) const {
        bool found = false;
        for (int i = 0; i < count; i++) {
            if (listings[i]->isApproved() && listings[i]->getVehicle() != nullptr) {
                if (listings[i]->getVehicle()->matchesFilter(key)) {
                    listings[i]->display();
                    found = true;
                }
            }
        }
        if (!found)
            cout << "No results for: " << key << endl;
    }

    void filterByPrice(float minP, float maxP) const {
        bool found = false;
        for (int i = 0; i < count; i++) {
            if (listings[i]->isApproved() && listings[i]->getVehicle() != nullptr) {
                float p = listings[i]->getVehicle()->getPrice();
                if (p >= minP && p <= maxP) {
                    listings[i]->display();
                    found = true;
                }
            }
        }
        if (!found)
            cout << "No listings in that price range.\n";
    }

    void showTotalListings() const {
        cout << "Total listings ever created: " << Listing::getTotalListings() << endl;
    }

    friend void showPlatformSummary(const Marketplace& m);
};

void showPlatformSummary(const Marketplace& m) {
    int approved = 0;
    float totalValue = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.listings[i]->isApproved()) {
            approved++;
            if (m.listings[i]->getVehicle() != nullptr)
                totalValue += m.listings[i]->getVehicle()->getPrice();
        }
    }
    cout << "\n--- Platform Summary ---\n";
    cout << "Total Listings: " << m.count << endl;
    cout << "Approved: " << approved << endl;
    cout << "Total Listed Value: Rs." << totalValue << endl;
}

int main() {

    Marketplace market;
    Admin admin(1, "Admin", "admin@market.com");
    Seller seller(2, "Ahmed", "ahmed@gmail.com");
    Buyer buyer(3, "Ali", "ali@gmail.com");

    User* users[3];
    users[0] = &admin;
    users[1] = &seller;
    users[2] = &buyer;

    cout << "\n--- Role Profiles ---\n";
    for (int i = 0; i < 3; i++)
        users[i]->showProfile();

    int roleChoice;

    do {
        cout << "\n===== CAR MARKETPLACE SYSTEM =====\n";
        cout << "1. Admin\n";
        cout << "2. Seller\n";
        cout << "3. Buyer\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> roleChoice;

        if (roleChoice == 1) {
            int ch;
            do {
                cout << "\n--- ADMIN PANEL ---\n";
                cout << "1. Approve Listing\n";
                cout << "2. View All Approved\n";
                cout << "3. Platform Summary\n";
                cout << "4. View Bids on Listing\n";
                cout << "0. Back\n";
                cout << "Choice: ";
                cin >> ch;

                if (ch == 1) {
                    int id;
                    cout << "Listing ID: ";
                    cin >> id;
                    Listing* l = market.getById(id);
                    if (l != nullptr)
                        admin.approveListing(*l);
                    else
                        cout << "Not found.\n";
                }
                else if (ch == 2) {
                    market.viewApproved();
                }
                else if (ch == 3) {
                    showPlatformSummary(market);
                    market.showTotalListings();
                }
                else if (ch == 4) {
                    int id;
                    cout << "Listing ID: ";
                    cin >> id;
                    Listing* l = market.getById(id);
                    if (l != nullptr)
                        l->showBids();
                    else
                        cout << "Not found.\n";
                }
            } while (ch != 0);
        }

        else if (roleChoice == 2) {
            int ch;
            do {
                cout << "\n--- SELLER PANEL ---\n";
                cout << "1. Add Car Listing\n";
                cout << "2. Add Bike Listing\n";
                cout << "3. View My Listings\n";
                cout << "4. View Inbox\n";
                cout << "5. View My Reviews\n";
                cout << "0. Back\n";
                cout << "Choice: ";
                cin >> ch;

                if (ch == 1) {
                    int id, yr, mile, doors;
                    float price;
                    string brand, model, style;

                    cout << "Listing ID: "; cin >> id;
                    cout << "Brand: "; cin >> brand;
                    cout << "Model: "; cin >> model;
                    cout << "Year: "; cin >> yr;
                    cout << "Mileage: "; cin >> mile;
                    cout << "Price: "; cin >> price;
                    cout << "Doors: "; cin >> doors;
                    cout << "Body Style (Sedan/SUV/Hatchback): "; cin >> style;

                    Car* c = new Car(brand, model, yr, mile, price, doors, style);
                    Listing* l = new Listing(id, c, seller.getId());
                    market.addListing(l);
                    seller.addListing(l);
                    cout << "Car listing added (Pending Approval).\n";
                }
                else if (ch == 2) {
                    int id, yr, mile, cc;
                    float price;
                    string brand, model, style;

                    cout << "Listing ID: "; cin >> id;
                    cout << "Brand: "; cin >> brand;
                    cout << "Model: "; cin >> model;
                    cout << "Year: "; cin >> yr;
                    cout << "Mileage: "; cin >> mile;
                    cout << "Price: "; cin >> price;
                    cout << "CC: "; cin >> cc;
                    cout << "Style (Sport/Cruiser): "; cin >> style;

                    Bike* b = new Bike(brand, model, yr, mile, price, cc, style);
                    Listing* l = new Listing(id, b, seller.getId());
                    market.addListing(l);
                    seller.addListing(l);
                    cout << "Bike listing added (Pending Approval).\n";
                }
                else if (ch == 3) {
                    seller.viewMyListings();
                }
                else if (ch == 4) {
                    seller.viewInbox();
                }
                else if (ch == 5) {
                    seller.showReviews();
                    seller.showRating();
                }
            } while (ch != 0);
        }

        else if (roleChoice == 3) {
            int ch;
            do {
                cout << "\n--- BUYER PANEL ---\n";
                cout << "1. View Approved Listings\n";
                cout << "2. Filter by Type (Car/Bike)\n";
                cout << "3. Filter by Brand/Model\n";
                cout << "4. Filter by Price Range\n";
                cout << "5. Add to Favorites\n";
                cout << "6. View Favorites\n";
                cout << "7. Place Bid\n";
                cout << "8. Message Seller\n";
                cout << "9. Leave Review\n";
                cout << "10. View Inbox\n";
                cout << "0. Back\n";
                cout << "Choice: ";
                cin >> ch;

                if (ch == 1) {
                    market.viewApproved();
                }
                else if (ch == 2) {
                    string t;
                    cout << "Type (Car/Bike): ";
                    cin >> t;
                    market.filterByType(t);
                }
                else if (ch == 3) {
                    string key;
                    cout << "Enter brand or model: ";
                    cin >> key;
                    market.filterByKeyword(key);
                }
                else if (ch == 4) {
                    float mn, mx;
                    cout << "Min price: "; cin >> mn;
                    cout << "Max price: "; cin >> mx;
                    market.filterByPrice(mn, mx);
                }
                else if (ch == 5) {
                    int id;
                    cout << "Listing ID: "; cin >> id;
                    Listing* l = market.getById(id);
                    if (l != nullptr && l->isApproved())
                        buyer.addFavorite(l);
                    else
                        cout << "Not found or not approved.\n";
                }
                else if (ch == 6) {
                    buyer.viewFavorites();
                }
                else if (ch == 7) {
                    int id;
                    float amt;
                    cout << "Listing ID: "; cin >> id;
                    cout << "Bid amount: "; cin >> amt;
                    Listing* l = market.getById(id);
                    if (l != nullptr)
                        l->placeBid(buyer.getId(), amt);
                    else
                        cout << "Listing not found.\n";
                }
                else if (ch == 8) {
                    string msg;
                    cin.ignore();
                    cout << "Message: ";
                    getline(cin, msg);
                    Message m(buyer.getId(), msg);
                    seller.getInbox()[seller.getMsgCount()++] = m;
                    cout << "Message sent.\n";
                }
                else if (ch == 9) {
                    int stars;
                    string comment;
                    cout << "Stars (1-5): "; cin >> stars;
                    cin.ignore();
                    cout << "Comment: ";
                    getline(cin, comment);
                    buyer.leaveReview(buyer.getId(), stars, comment, seller.getReviews(), seller.getReviewCount());
                }
                else if (ch == 10) {
                    buyer.viewInbox();
                }
            } while (ch != 0);
        }

    } while (roleChoice != 0);

    cout << "Exiting...\n";
    return 0;
}