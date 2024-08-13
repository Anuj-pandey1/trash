
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
#include<set>
#include<vector>
#include<map>
#include<set>
#include<queue>

using namespace std;

class Train {
public:
    int trainId;
    string trainName;
    int seatCount;
    map<string, string> route; 

    Train(int trainId , string trainName , int seatCount) {
        this->trainId = trainId;
        this->trainName = trainName;
        this->seatCount = seatCount;
    }
};

map<pair<string , string> , set<int>> trainsServiceable; // [station , date] -> set of trains;
map<string , vector<string>> stationGraph;
map<int , Train*> trainMap; // trainId -> Train Obj


void addTrain(int trainId, string trainName, vector<string> stations , vector<string> schedule , int seatCount) {
    Train* train = new Train(trainId , trainName , seatCount);
    trainMap[trainId] = train;
    int n = stations.size();
    for(int i=1;i<n;i++) {
        stationGraph[stations[i-1]].push_back(stations[i]);
        for(string date : schedule) {
            trainsServiceable[{stations[i] , date}].insert(trainId);
        }
    }
}

void viewAvailableTrains(string source, string destination, string date) {
    // logic-1
    queue<string> q;
    q.push(source);
    while(!q.empty()) {
        string curr = q.front();
        q.pop();
        for(string c : stationGraph[curr]) {
            if(c == destination) {
                vector<int> trainList;
                if(trainsServiceable.find({destination , date}) == trainsServiceable.end()) {
                    cout<<"no trains"<<endl;
                    return;
                }
                cout << "Available Trains {id, name} :> ";
                for(int trainId : trainsServiceable[{destination , date}]) {
                    cout<< "(" <<trainId << ", " << trainMap[trainId]->trainName << ") ";
                }
                cout<<endl;
                return;
            }
            q.push(c);
        }
    }
    cout<<"no trains"<<endl;

    // logic-2
    if(trainsServiceable.find({source, date}) == trainsServiceable.end()){
        cout << "No trains available\n";
        return;
    }
    set<int> totalTrains = trainsServiceable.find({source, date})->second;
    vector<int> availableTrains;

    
}


int checkSeatAvailability(int trainId, string source, string destination, string date) {
    Train* train = trainMap[trainId];


    
}


void bookTicket(int trainId, string source, string destination, string date) {
    Train* train = trainMap[trainId];


    
}

void cancelTicket(int ticketId){

}
void listBookedTickets(int trainId, string date){

}

int main() {
    addTrain(1 , "t1" , {"a" , "b" , "c"} , {"d1" , "d2" , "d3"} , 2);
    addTrain(2 , "t2" , {"a" , "c"} , {"d1" , "d3"} , 3);

    viewAvailableTrains("a" , "c" , "d2");
    cout<<checkSeatAvailability(1 , "a" , "c" , "d1")<<endl;
    bookTicket(1 , "a" , "c" , "d1");
    cout<<checkSeatAvailability(1 , "a" , "c" , "d1")<<endl;
}


// Time and Space Complexity Discussion --------------->
