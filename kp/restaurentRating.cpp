#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<unordered_map>

using namespace std;

class Restaurent {
    public:
    int maxOrders;
    map<string , int> menu; //dish name -> quantity;
    map<string , int> itemPrice; //dish -> price
    int id;
    int rating;
    int currServing = 0;

    Restaurent(int maxOrders , map<string , int> menu , map<string , int> itemPrice , int id , int rating) {
        this->maxOrders = maxOrders;
        this->menu = menu;
        this->itemPrice = itemPrice;
        this->id = id;
        this->rating = rating;
    }

    void updateMenu(string dish , int price , int quantity) {
        menu[{dish , price}] += quantity;
    }

    bool canServe(set<pair<string , int>> order) {
        if(this->currServing >= this->maxOrders) return false;
        for(auto item : order) {
            if((menu.find(item.first) == menu.end()) || (menu[item.first] < item.second)) return false;
        }
        return true;
    }

    int getPrice(set<pair<string , int>> order) {
        int totalPrice = 0;
        for(auto item : order) {
            int dishPrice = itemPrice[item.first];
            int dishQuantity = item.second;
            totalPrice += (dishPrice*dishQuantity);
        }
        return totalPrice;
    }

    void updateAmount(set<pair<string , int>> order) {
        for(auto item : order) {
            menu[item.first] -= item.second;
        }
        return;
    }
};

class Order {
    public:
    Restaurent* restaurent;
    int id;
    set<pair<string , int>> order;
    int bill;

    Order(int id , set<pair<string , int>> order) {
        this->id = id;
        this->order = order;
    }
};

struct RatingComp {
    bool operator()(Restaurent* r1 , Restaurent* r2) const{
        if(r1->rating != r2->rating) return r1->rating > r2->rating;
        else {
            return r1->id < r2->id;
        }
    }
};

map<int , Restaurent*> restaurentMap; //restaurentid -> restaurent object
map<int , Order*> orderMap; //orderId -> restaurent
set<Restaurent* , RatingComp> ratingSortSet; //restarurents sorted according to their rating;


//registering restaurent
void registerRestaurent(int id , int maxOrders , map<string , int> menu , map<string , int> itemPrice , int rating) {
    Restaurent* restaurent = new Restaurent(maxOrders , menu , itemPrice , id , rating);
    restaurentMap[id] = restaurent;
    ratingSortSet.insert(restaurent);
}

//placing order from the highest rated restaurent
void placeOrderByRating(set<pair<string , int>> order , int orderId) {
    Restaurent* bestRestaurent = NULL;
    Order* currOrder = new Order(orderId , order);

    for(auto restaurent : ratingSortSet) {
        if(restaurent->canServe(order)) {
            bestRestaurent = restaurent;
            break;
        }
    }
    if(bestRestaurent == NULL) {
        cout<<"Order can't be served"<<endl;
    }
    else {
        bestRestaurent->updateAmount(order);
        bestRestaurent->currServing += 1;
        cout<<"Order Placed from R"<<bestRestaurent->id<<endl;
        currOrder->restaurent = bestRestaurent;
        currOrder->bill = bestRestaurent->getPrice(order);
    }
}

//placing order from the lowest cost restaurent
void placeOrderByPrice(set<pair<string , int>> order , int orderId) {
    int bestCost = INT_MAX;
    Restaurent* bestRestaurent = NULL;
    Order* currOrder = new Order(orderId , order);

    for(auto restaurent : ratingSortSet) {
        if(restaurent->canServe(order)) {
            int cost = restaurent->getPrice(order);
            if(cost < bestCost) {
                bestCost = cost;
                bestRestaurent = restaurent;
            }
        }
    }

    if(bestRestaurent == NULL) {
        cout<<"Order can't be served"<<endl;
    }
    else {
        bestRestaurent->updateAmount(order);
        bestRestaurent->currServing += 1;
        cout<<"Order Placed From R"<<bestRestaurent->id<<endl;
        currOrder->restaurent = bestRestaurent;
        currOrder->bill = bestCost;
    }
}

//marking the order as complete
void updateOrder(int orderId) {
    Order* currOrder = orderMap[orderId];
    currOrder->restaurent->currServing -= 1;
    cout<<"Order completed"<<endl;
    cout<<"Bill: "<<currOrder->bill<<endl;
}

int main() {
    registerRestaurent(1 , 2 , {{"paneer" , 2} , {"chicken" , 2}} , {{"paneer" , 100} , {"chicken" , 200}} , 4.6);
    registerRestaurent(2 , 1 , {{"paneer" , 1} , {"chicken" , 2}} , {{"paneer" , 90} , {"chicken" , 250}} , 4.8);
    registerRestaurent(3 , 2 , {{"paneer" , 1} , {"rice" , 2}} , {{"paneer" , 110} , {"rice" , 50}} , 4.4);
    registerRestaurent(4 , 2 , {{"paneer" , 1} , {"rice" , 2}} , {{"paneer" , 100} , {"rice" , 30}} , 4.3);
    
    placeOrderByPrice({{"paneer" , 1} , {"rice" , 2}} , 2);

    cout<<ratingSortSet.size()<<endl;
}