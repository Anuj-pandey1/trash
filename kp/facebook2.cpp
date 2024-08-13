/*
createPost()
deletePost()
follow()
unfollow()
getFeed()
getFeedPaginated()
getTopNPost()
*/

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

class GlobalTime
{
public:
    static int getTime()
    {
        static int time = 0;
        return time++;
    }
};

class Post
{
public:
    int postId;
    int postTime;
    Post *prevPost;
    Post *nextPost;

    Post(int postId)
    {
        this->postId;
        this->postTime = GlobalTime().getTime();
        prevPost = NULL;
        nextPost = NULL;
    }
};

class User
{
public:
    int userId;
    string userName;
    set<int> followed;

    User(int userId, string userName)
    {
        this->userId = userId;
        this->userName = userName;
    }
};

class Facebook
{
    map<int, set<int>> friends;            //
    map<int, Post *> posts;                // userId -> head of post*
    map<int, User *> users;                // userId -> User obj
    map<int, pair<Post *, int>> postAddrs; // postId -> Post* , userId

public:
    void follow(int followerUserId, int followedUserId)
    {
        friends[followerUserId].insert(followedUserId);
    }
    void unfollow(int followerUserId, int followedUserId)
    {
        friends[followerUserId].erase(followedUserId);
    }

    void createPost(int userId, int postId)
    {
        Post *post = new Post(postId);
        postAddrs[postId] = {post, userId};
        if (posts.find(userId) != posts.end())
        {
            posts[userId]->prevPost = post;
            post->nextPost = posts[userId];
        }
        posts[userId] = post;
    }

    void deletePost(int postId)
    {
        Post *post = postAddrs[postId].first;
        int userId = postAddrs[postId].second;
        postAddrs.erase(postId);

        // if it is a head
        if (post->prevPost == NULL)
        {
            // if it is a single post
            if (post->nextPost == NULL)
            {
                posts.erase(userId);
            }
            else
            {
                post->nextPost->prevPost = NULL;
                posts[userId] = post->nextPost;
            }
        }
        else
        {
            // if it is a last post
            if (post->nextPost == NULL)
            {
                post->prevPost->nextPost = NULL;
            }
            else
            {
                post->nextPost->prevPost = post->prevPost;
                post->prevPost->nextPost = post->nextPost;
            }
        }
    }

    void getPost(int userId)
    {
        auto comp = [](Post *post1, Post *post2)
        { return post1->postTime > post2->postTime; }; // lambda function

        multiset<Post*, decltype(comp)> postSet(comp);

        for(auto connections: friends[userId]){
            postSet.insert(posts[connections]);
        }

        vector<int> latestPosts; // postId

        while(postSet.size() > 0 and )

    }
};

int main()
{
    return 0;
}