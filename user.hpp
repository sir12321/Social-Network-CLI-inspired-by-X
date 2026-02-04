#include <unordered_set>
#include "posts.hpp"

using namespace std;

class User {
    private:
        string user_name;
        AVLTree posts_tree;
        unordered_set<User*> friends;
    public:
        User(const string& name) : user_name(name) {}

        // addPost: insert a new post into the user's AVL tree.
        // The underlying AVLTree assigns an incremental id for storage.
        void addPost(const string& content) {
            posts_tree.insert(content);
        }

        string getName() const {
            return user_name;
        }

        // getPosts: retrieve up to N posts (via reverse-inorder traversal)
        // and print them to stdout. This mirrors the OUTPUT_POSTS command
        // behavior in the CLI program.
        void getPosts(int N) {
            vector<string> posts = posts_tree.traversal_post(N);
            cout << "Posts of " << user_name << " :\n";
            for (const string& post_content : posts) {
                cout << "    " << "Post Content: " << post_content << "\n";
            }
        }

        // addFriend: insert a pointer to another User into the friend set.
        // This code assumes friendship links are established symmetrically
        // by the caller (i.e., both users are added to each other's set).
        void addFriend(User* user) {
            friends.insert(user);
        }

        // Return a copy of the friends set. Using a copy avoids exposing
        // the internal set to external mutation; callers operate on the
        // returned set for lookups/traversal.
        unordered_set<User*> getFriends() const {
            return friends;
        }

        // cout_Friends: print the list of friends to stdout. This is a
        // convenience used by the LIST_FRIENDS CLI command.
        void cout_Friends() {
            cout << "Friends of " << user_name << ":\n";
            vector<string> fri;
            for(auto i : friends){
                fri.push_back(i->user_name);
            }
            sort(fri.begin(),fri.end());
            for (const auto& friend_user : fri) {
                cout << "    " << friend_user << "\n";
            }
        }
};
