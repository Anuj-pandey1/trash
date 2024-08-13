// 1. addTrain(trainId, trainName, stations, schedule)
        // ○ Adds a train with the given details.

        // ○ trainId (String): Unique identifier for the train.
        // ○ trainName (String): Name of the train.
        // ○ stations (List of Strings): List of stations in sequence.
        // ○ schedule (List of Strings): List of dates on which the train runs in 'YYYY-MM-DD' format.


// 2. viewAvailableTrains(source, destination, date)
        // ○ Returns a list of trains available between the source and destination on the specified date.

        // ○ source (String): Source station.
        // ○ destination (String): Destination station.
        // ○ date (String): Date of travel in 'YYYY-MM-DD' format.


// 3. checkSeatAvailability(trainId, source, destination, date)
        // ○ Returns the number of available seats for the specified train for the journey
        // between source and destination on the given date.

        // ○ trainId (String): Unique identifier for the train.
        // ○ source (String): Source station.
        // ○ destination (String): Destination station.
        // ○ date (String): Date of travel in 'YYYY-MM-DD' format.


// 4. bookTicket(trainId, source, destination, date)
        // ○ Books a ticket for the specified train for the journey between source and
        // destination on the given date if seats are available. Returns a unique ticket ID.

        // ○ trainId (String): Unique identifier for the train.
        // ○ source (String): Source station.
        // ○ destination (String): Destination station.
        // ○ date (String): Date of travel in 'YYYY-MM-DD' format.

// BOUNS METHODS--------------------->>>>>

// 1. cencelTicket (ticketId)
        // cencels a booked ticket with the specified ticket ID. Returns a confirmation or failure message.
        // ticketId(stringId): Unique Identifier for the ticket

// 2. listBookedTickets(trainId, date)
        // Return a list of booked ticket with the specified train on the given date.
        // trainId(String): Unique Identifier for the train.
        // date(string): Date of travel in 'YYYY-MM-DD' format.
///////////////////////////////////////////////////////////////////////////////////////

#include<iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Train{
public:
    int trainID;
    string trainName;
    int seats;

    Train(int trainID, string trainName, int seats){
        this->trainID = trainID;
        this->trainName = trainName;
        this->seats = seats;
    }
};


class IRCTC{
    map<int, Train*> trainMap; // trainId = train obj
    map<string, vector<string>> stationGraph; 
    map<pair<string , string> , set<int>> stationTrains; // {station, date} -> trains

    map<pair<int, string>, int> ticketCount; // {trainId, date} -> available tickets;

    map<int, pair<int, string>> ticketMap; // ticketId -> {trainId, date}
    map<pair<int, string>, set<int>> listTickets; // {trainId, date} -> set of ticketId;

    int UniqueIdGenerator(){
        static int id = 0;
        return id++;
    }
public:
    // 
    void addTrain(int trainID, string trainName, vector<string> stations, vector<string> schedules){
        Train* train = new Train(trainID, trainName, 100);
        trainMap[trainID] = train;

        for(int i=1; i<stations.size(); i++){
            stationGraph[stations[i-1]].push_back(stations[i]);
        }

        for(auto station: stations){
            for(auto schedule: schedules){
                stationTrains[{station, schedule}].insert(trainID);
            }
        }
        
        for(auto schedule: schedules){
            ticketCount[{trainID, schedule}] = train->seats;
        }
    }

    set<int> viewAvailableTrains(string source, string destination, string date){
        set<int> availableTrains;

        if(stationTrains.find({source, date}) == stationTrains.end()){
            cout << "No trains available\n";
            return availableTrains;
        }
        set<int> sourceTrains = stationTrains.find({source, date})->second;
        set<int> destinationTrains = stationTrains.find({destination, date})->second;


        for(auto trainId: sourceTrains){
            if(destinationTrains.find(trainId) != destinationTrains.end()){
                availableTrains.insert(trainId);
            }
        }

        if(availableTrains.size() == 0){
            cout << "No trains available\n";
            return availableTrains;
        }

        cout << "Available Trains:-----\n";
        for(auto trainId: availableTrains){
            cout << "\ttrainID: " << trainId << endl;
        }

        return availableTrains;
    }

    bool checkSeatAvailability(int trainID, string source, string destination, string date){
        set<int> availableTrains = viewAvailableTrains(source, destination, date);
        
        if(availableTrains.find(trainID) == availableTrains.end()){
            cout << "Train is not available b/w given route on given date\n";
            return false;
        }
        
        Train* train = trainMap[trainID];
        if(ticketCount[{trainID, date}] == 0){
            cout << "No seats available\n";
            return false;
        }
        cout << "no. available seats is: " << ticketCount[{trainID, date}] << endl;
        return true;
    }

    int bookTicket(int trainID, string source, string destination, string date){
        bool checkForSeats = checkSeatAvailability(trainID, source, destination, date);
        if(checkForSeats == true){
            Train* train = trainMap[trainID];
            ticketCount[{trainID, date}]--;
            int ticketId = UniqueIdGenerator();

            ticketMap[ticketId] = {trainID, date};
            listTickets[{trainID, date}].insert(ticketId);

            cout << "Your Ticket is booked, TicketId: " << ticketId << endl;
            return ticketId;
        }

        cout << "Ticket can't be booked\n";
        return -1;
    }

    void cancelTicket(int ticketId){
        if(ticketMap.find(ticketId) == ticketMap.end()){
            cout << "Invalid Ticket Id\n";
            return;
        }
        int trainId = ticketMap[ticketId].first;
        string date = ticketMap[ticketId].second;

        ticketCount[{trainId, date}]++;

        ticketMap.erase(ticketId);
        listTickets[{trainId, date}].erase(ticketId);
        
        cout << "Ticket cancelled successfully\n";
    }

    void listBookedTickets(int trainId, string date){
        cout << "List of booked tickets(ticketIds): ";
        for(auto ticketId: listTickets[{trainId, date}]){
            cout << ticketId << ", ";
        }
        cout << endl;
    }
};


int main(){
    cout << "Lets have fun----------\n\n";

    IRCTC* irctc = new IRCTC();

    irctc->addTrain(1, "t1", {"s1", "s2", "s3"}, {"d1", "d2", "d3"});
    irctc->addTrain(2, "t2", {"s1", "s3"}, {"d1", "d3"});
    
    irctc->viewAvailableTrains("s1", "s3", "d2");
    irctc->checkSeatAvailability(1, "s1", "s3", "d1");
    int ticketId = irctc->bookTicket(1, "s1", "s3", "d1");
    irctc->checkSeatAvailability(1, "s1", "s3", "d1");

    irctc->listBookedTickets(1, "d1");
    irctc->cancelTicket(ticketId);
    irctc->listBookedTickets(1, "d1");

    return 0;
}