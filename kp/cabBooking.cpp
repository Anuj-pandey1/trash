/*
            ** Cab Booking Application **

Description:-
    :- Implement a cab booking application. Below are the expected features from the system.

Features:
    :- The application allows users to book rides on a route.
    :- Users can register themself and make changes to their details.
    :- Driving partner can onboard on the system with the vehicle details.
    :- Users can search and select one from multiple available rides 
        on a route with the same source and destination based on the nearest to the user.

Requirements:
    Application should allow user onboarding.
    add_user(user_detail)

    Add basic user details
    update_user(username, updated_details)
    User should be able to update its contact details

    update_userLocation(username,Location):
    This will update the user location in {X, Y} coordinate to find nearest in future

    Application should allow Driver onboarding
    add_driver(driver_details,vehicle_details,current_location)
    This will create an instance of the driver and will mark his current location on the map

    update_driverLocation(driver_name, location)
    This will mark the current location of driver 

    change_driver_status(driver_name,status)
    In this driver can make himself either available or unavailable via a boolean
      
    Application should allow the user to find a ride based on the criteria below
    find_ride (Username, Source, destination)
    It will return a list of available ride 

    choose_ride(Username,driver_name)
    It will choose the drive name from the list

    Note : Only the driver which is at a max distance of 5 unit will be displayed to a user and 
        the driver should be in available state to confirm the booking

    calculateBill(Username):
    It will return the bill based on the distance between the source and destination and will display it    

    Application should at the end calculate- the earning of all the driver onboarded in the application 
    find_total_earning()
*/
// --------------------------------------------------------------------------------------

/*
classes - User, Driver, Location, Vehicle

*/


///////////////////////////////////////////   code   //////////////////////////////////////////

#include<iostream>
#include <vector>
#include<map>
#include<unordered_map>
#include <set>
#include <queue>
using namespace std;

class Location{
public:
    int x;
    int y;

    Location(int x, int y){
        this->x = x;
        this->y = y;
    }
};

class Vehicle{
public:
    string name;
    int capacity;
    
    Vehicle(string name, int capacity){
        this->name = name;
        this->capacity = capacity;
    }
};

class User{
public:
    string userName; // unique Identifier
    string name;
    string email;
    string mobileNumber;
    Location* location;

    User(string userName, string name, string email, string mobileNumber, Location* location){
        this->userName = userName;
        this->name = name;
        this-> email = email;
        this->mobileNumber = mobileNumber;
        this->location = location;
    }
};

class Driver{
public:
    string driverName;
    string name;
    string email;
    string mobileNumber;
    Location* location;
    Vehicle* vehicle;
    bool status;
    int earning;

    Driver(string driverName, string name, string email, string mobileNumber, 
            Location* location, Vehicle* vehicle, bool status){
        this->driverName = driverName;
        this->name = name;
        this->email = email;
        this->mobileNumber = mobileNumber;
        this->location = location;
        this->vehicle = vehicle;
        this->status = status;
        this->earning = 0;
    }
};

// ------------ Methods ---------
map<string, User*> users; // userName -> User Obj
map<string, Driver*> drivers; // driverName -> Driver Obj

map<Location*, set<Driver*>> driversCoordinates;



void add_user(User* user){
    users[user->userName] = user;
}

void update_user(string userName, map<string, string> updateDetails){
    if(users.find(userName) == users.end()){
        cout << "Invalid userName\n";
        return;
    }
    User* user = users[userName];

    for(auto updateDetail: updateDetails){
        string key = updateDetail.first;
        string value = updateDetail.second;
        
        if(key == "email") user->email = value;
        else if(key == "mobileNumber") user->mobileNumber = value;
    }
    cout << "user details updated successfully\n";
}

void update_userLocation(string userName, Location* location){
    if(users.find(userName) == users.end()) {
        cout << "Invalid User Name\n";
        return;
    }
    User* user = users[userName];
    user->location = location;
}

void add_driver(Driver* driver){
    drivers[driver->driverName] = driver;
    // driversCoordinates.insert({driver->location, driver});
}

void update_driverLocation(string driverName, Location* location){
    if(drivers.find(driverName) == drivers.end()){
        cout << "Invalid Driver Name\n";
        return;
    }
    Driver* driver = drivers[driverName];
    driver->location = location;
}

void change_driver_status(string driverName, bool status){
    if(drivers.find(driverName) == drivers.end()){
        cout << "Invalid Driver Name\n";
        return;
    }
    Driver* driver = drivers[driverName];
    driver->status = status;
}

int calculate_bill(Location* source, Location* destination){
    int distance = abs(source->x - destination->x) + abs(source->y - destination->y);
    int cost = distance*10;
    cout << "cost for ride: " << cost << endl;
    return cost;
}

// lets choose the closes one - 
void choose_ride(string userName, vector<Driver*> rides, int bill){
    if(users.find(userName) == users.end()) {
        cout << "Invalid User Name\n";
        return;
    }
    User* user = users[userName];

    int closestDistance = 1e9;
    Driver* choosenDriver = NULL;
    for(Driver* driver: rides){
        int distance = abs(user->location->x - driver->location->x) + abs(user->location->y - driver->location->y);
        if(closestDistance > distance){
            closestDistance = distance;
            choosenDriver = driver;
        }
    }
    choosenDriver->earning = choosenDriver->earning + bill;
    cout << "Driver Found-\n";
    cout << "   name- " << choosenDriver->driverName << endl;
    cout << "   phone- " << choosenDriver->mobileNumber << endl;
}

// only riders that are close than 5 unit of distance (lets use manhattan distance)
void find_ride(string userName, Location* source, Location* destination){
    if(users.find(userName) == users.end()) {
        cout << "Invalid User Name\n";
        return;
    }
    User* user = users[userName];



    // Optimize
    vector<Driver*> res;
    queue<pair<pair<int, int>, int>> q;
    q.push({source, 0});
    set<pair<int, int>> visited;
    visited.insert({source->x, source->y});

    vector<pair<int, int>> move = {{0,1}, {1,0}, {0,-1}, {-1,0}};

    while(q.size() > 0){
        pair<int, int> currLoc = q.front().first;
        int dist = q.front().second;
        q.pop();


        if(driversCoordinates.find(currLoc) != driversCoordinates.end()){
            set<Driver*> dr = driversCoordinates[currLoc];
            for(auto d: dr){
                if(d->status == true){
                    res.push_back(d);
                }
            }
        }
        
        for(int i=0; i<4; i++){
            pair<int, int> pr
            int dist = abs(newLoc->x - source->x) + abs(newLoc->y - source->y);
            if(dist < 5 && visited.find(newLoc) == visited.end()){

            }
        }
    }


    vector<Driver*> rides;


    for(auto pair: drivers){
        Driver* driver = pair.second;
        if(driver->status == false) continue;
        int distance = abs(source->x - driver->location->x) + abs(source->y - driver->location->y);
        if(distance <= 5) rides.push_back(driver);
    }
    if(rides.size() == 0) {
        cout << "No Drivers available in your area\n";
        return;
    }
    int bill = calculate_bill(source, destination);
    choose_ride(userName, rides, bill);
}



void find_total_earning(){
    cout << "List of total earning---\n";
    for(auto pair: drivers){
        Driver* driver = pair.second;
        cout << "Driver: " << driver->driverName << ", earning: " << driver->earning << endl;
    }
}

int main(){
    // User(string userName, string name, string email, string mobileNumber, Location* location)
    User *u1 = new User("u1", "User-1", "u@1.com", "123", new Location(0, 0));
    User *u2 = new User("u2", "User-2", "u@2.com", "234", new Location(10, 0));
    User *u3 = new User("u3", "User-3", "u@3.com", "345", new Location(15, 6));
    
    // Driver(string driverName, string name, string email, string mobileNumber, 
            // Location* location, Vehicle* vehicle, bool status)
    Driver *d1 = new Driver("d1", "Driver-1", "d1@g.com", "456", new Location(10, 1), new Vehicle("v1", 1), true);
    Driver *d2 = new Driver("d2", "Driver-2", "d1@g.com", "456", new Location(11, 10), new Vehicle("v2", 2), true);
    Driver *d3 = new Driver("d3", "Driver-3", "d1@g.com", "456", new Location(5,4), new Vehicle("v3", 3), true);
    
    
    add_user(u1);
    add_user(u2);
    add_user(u3);

    add_driver(d1);
    add_driver(d2);
    add_driver(d3);
    
    find_ride("u2", new Location(2,3), new Location(6,8));
    
    find_total_earning();
    return 0;
}