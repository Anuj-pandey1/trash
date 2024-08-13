#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<unordered_map>

using namespace std;

class Product {
    public:
    string type;
    int price;
    int productId;
    int sellerId;

    Product(string type , int price , int productId , int sellerId) {
        this->type = type;
        this->price = price;
        this->productId = productId;
        this->sellerId = sellerId;
    }
};

class Seller {
    public:
    int sellerId;
    set<int> pincodes;

    Seller(int sellerId , set<int> pincodes) {
        this->sellerId = sellerId;
        this->pincodes = pincodes;
    }
};

//for a particular seller keep the product id same for same type of product
unordered_map<int , set<Product*>> inventory; //sellerId -> setof Product;
unordered_map<pair<int , int> , int> quantityMap; // {productId , selledId} -> quantity;
unordered_map<int , Product*> productMap; // prodcutId -> Product object
unordered_map<int , Seller*> sellerMap; //sellerId -> Seller object

struct customComp {
    bool operator()(Product* p1 , Product* p2) {
        return p1->price < p2->price;
    }
};

map<pair<string , int> , set<Product* , customComp>> productStock; // {productType , pincode} -> set of Products .. should be sorted according to the product price;


//adding some quantity of a product
void addInventory(int productId , int sellerId , int quantity) {
    Product* product = productMap[productId];
    Seller* seller = sellerMap[sellerId];

    inventory[sellerId].insert(product);
    if(quantityMap.find({sellerId , productId}) != quantityMap.end())  quantityMap[{sellerId , productId}] += quantity;
    else quantityMap[{sellerId , productId}] = quantity;

    
    for(int pincode : seller->pincodes) {
        productStock[{product->type , pincode}].insert(product);
    }
    cout<<"Item added to inventory"<<endl;
}


//creatng seller
void createSeller(int sellerId , set<int> pincodes) {
    Seller* seller = new Seller(sellerId , pincodes);
    sellerMap[sellerId] = seller;
} 

//creating product
void createProduct(int productId , int sellerId , int price , string type) {
    Product* product = new Product(type , price , productId , sellerId);
    productMap[productId] = product;
}

//getting the inventory of a seller
void getInventory(int sellerId) {
    for(Product* product : inventory[sellerId]) {
        int quantity = quantityMap[{product->productId , sellerId}];

        cout<<"Product Id: "<<product->productId<<" Type: "<<product->type<<" Price: "<<product->price<<" Quantity: "<<quantity<<endl;
    }
}

//getting best sellers for the product
void getSeller(string type , int pincode) {
    for(Product* product : productStock[{type , pincode}]) {
        cout<<"Product ID: "<<product->productId<<" Price: "<<product->price<<" Quantity: "<<quantityMap[{product->productId , product->sellerId}]<<endl;
    }
}

///creating the order decreasing the product from the inventory
void createOrder(int orderId , int pincode , int sellerId , int itemCount , int productId) {
    Seller* seller = sellerMap[sellerId];
    
    if(seller->pincodes.find(pincode) != seller->pincodes.end() && 
    quantityMap.find({productId , sellerId}) != quantityMap.end() && 
    quantityMap[{productId , sellerId}] >= itemCount) {
        cout<<"Order Placed successfully"<<endl;
        int newCount = quantityMap[{productId , sellerId}] - itemCount;
        quantityMap[{productId , sellerId}] = newCount;
        if(newCount == 0) {
            quantityMap.erase({productId , sellerId});
            inventory[sellerId].erase(productMap[productId]);
            productStock[{productMap[productId]->type , pincode}].erase(productMap[productId]);
        }
    } 
    else{
        cout<<"Can't place order"<<endl;
    }
}

int main() {

}