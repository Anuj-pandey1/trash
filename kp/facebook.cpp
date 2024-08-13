#include<iostream>
#include<vector>
#include<set>
#include<map>

using namespace std;

int gtime = 0;

class Post {
    public:
    int time;
    Post* next;
    Post* prev;
    int postId;

    Post(int postId) {
        this->postId = postId;
        next = NULL;
        prev = NULL;
        gtime++;
        this->time = gtime;
    }
};

map<int , Post*> posts;  //userId -> Post*
map<int , pair<Post* , int>> postAdrs; //postId -> Post* , userId
map<int , set<int>> friends;

void follow(int followerId , int followeeId) {
    friends[followerId].insert(followeeId);
}

void unfollow(int followerId , int followeeId) {
    friends[followerId].erase(followeeId);
}

void post(int userId , int postId) {
    Post* p1 = new Post(postId);
    postAdrs[postId] = {p1 , userId};
    if(posts.find(userId) != posts.end()) {
        p1->prev = posts[userId]->prev; // looks waste
        posts[userId]->prev = p1;
        p1->next = posts[userId];
    }
    posts[userId] = p1;
}

vector<int> getPost(int userId) {
    friends[userId].insert(userId);

    auto comp = [](Post* p1 , Post* p2){return p1->time > p2->time;};

    multiset<Post* , decltype(comp)> ms(comp); // use Priority queue

    for(auto user : friends[userId]) {
        if(posts.find(user) != posts.end()) {
            ms.insert(posts[user]);
        }
    }

    vector<int> ans;

    for(int i=0;i<10 && !ms.empty();i++) {
        Post* latestP = *ms.begin();
        ms.erase(latestP);
        if(latestP->next != NULL) ms.insert(latestP->next);
        ans.push_back(latestP->postId);
    }

    return ans;
} 

void removePost(int postId) {
    Post* p = postAdrs[postId].first;
    int userId = postAdrs[postId].second;

    // if it is a head
    if(p->prev == NULL) {
        if(p->next == NULL) {
            posts.erase(userId);
        }
        else {
            p->next->prev = NULL;
            posts[userId] = p->next;
        }
    }
    
    else {
        if(p->next == NULL) {
            p->prev->next = NULL;
        }
        else {
            p->prev->next = p->next;
            p->next->prev = p->prev;
        }
    }
    free(p);
}

int main() {
    follow(1 , 2);
    follow(1 , 3);
    follow(1 , 4);

    post(1 , 1);
    post(2 , 2);
    post(2 , 3);
    post(2 , 4);
    post(3 , 5);
    post(3 , 6);
    post(4 , 8);


    vector<int> ans1 = getPost(1);
    for(auto it : ans1) cout<<it<<" ";
    cout<<endl;

    unfollow(1 , 4);
    vector<int> ans2 = getPost(1);
    for(auto it : ans2) cout<<it<<" ";
    cout<<endl;

    removePost(3);
    removePost(4);
    vector<int> ans3 = getPost(1);
    for(auto it : ans3) cout<<it<<" ";
    cout<<endl;
}