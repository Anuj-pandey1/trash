#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<string>

using namespace std;

class Driver {
public:
    int id;
    int seats;
    string name;

    Driver(int id , int seats , string name) {
        this->id = id;
        this->seats = seats;
        this->name = name;
    }
};

class Rider {
    public:
    int id;
    string name;
    int rides = 0;

    Rider(int id , string name) {
        this->id = id;
        this->name = name;
    }
};

class Ride {
    int id;
    int origin;
    int destination;
    int seats;
    Rider* rider;
    Driver* driver;

    Ride(Rider* rider , Driver* driver) {
        this->rider = rider;
        this->driver = driver;
    }

    void createRide(int id , int origin , int destination , int seats) {
        this->id = id;
        this->origin = origin;
        this->destination = destination;
        this->seats = seats;
        rider->rides++;
    }

    void updateRide(int origin , int destination , int seats) {
        this->origin = origin;
        this->destination = destination;
        this->seats = seats;
    }

    void closeRide() {
        Bill* bill = new Bill();
        if(rider->rides > 10) {
            bill->preferredBill(destination , origin , seats , rider , driver);
        }
        else {
            bill->normalBill(destination , origin , seats , rider , driver);
        }
    }
};

class Bill {
   public:
   void normalBill(int destination , int origin , int seats , Rider* rider , Driver* driver) {
    int totalPrice;
        if(seats == 1) {
            totalPrice = 20 * abs(destination - origin);
        }
        else {
            totalPrice = 20 * abs(destination - origin) * seats * .75;
        }

        cout<<"Ride Details "<<endl;
        cout<<"Rider: "<<rider->name<<" Driver: "<<driver->name<<" Price: "<<endl;
   }

   void preferredBill(int destination , int origin , int seats , Rider* rider , Driver* driver) {
    int totalPrice;
        if(seats == 1) {
            totalPrice = 20 * abs(destination - origin) * .75;
        }
        else {
            totalPrice = 20 * abs(destination - origin) * seats * .5;
        }

        cout<<"Ride Details "<<endl;
        cout<<"Rider: "<<rider->name<<" Driver: "<<driver->name<<" Price: "<<endl;
   }
};
//adding a driver


int main() {
    
}