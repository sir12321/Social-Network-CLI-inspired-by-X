#include <ctime>
#include <stdexcept>
#include <unordered_map>
#include <queue>
#include "user.hpp"

using namespace std;

string to_lower(const string &s){
    // Convert a string to lowercase. This program normalizes all
    // usernames to lowercase so that comparisons are case-insensitive.
    string a = "";
    for(char c : s){
        if (c >= 'A' && c <= 'Z') {
            a += (c + 32);
        } else {
            a += c;
        }
    }
    return a;
}

// Function to split a string by spaces into a vector of words (command parser)
// Example: "INSERT file1 Hello World" -> ["INSERT", "file1", "Hello", "World"]
// This helps in parsing user commands from input
vector<string> separate(const string &s){

    vector<string> result;
    string token;
    int count = 0;
    for(char c : s){
        bool ws = (c == ' ' || c == '\t' || c == '\n' );
        if(ws && count != 2){
            if(!token.empty())
            {
                result.push_back(token);
                token.clear();
                count ++;
            }
        } else {
            token.push_back(c);
        }
    }
    if(!token.empty()) result.push_back(token);
    return result;
}

int main(){

    unordered_map<string,User*> data_base;

    while (true)
    {
        try {
            // Read user input (command)
            string input;
            if(!getline(cin,input)) {
                break;
            }

            vector<string> command = separate(input);

            if (command.empty()) continue;

            // HELP: show usage
            if(command[0] == "HELP"){
                cout << "Available commands:\n"
                     << "  ADD_USER <username>\n"
                     << "  ADD_FRIEND <username1> <username2>\n"
                     << "  ADD_POST <username> <post_content>\n"
                     << "  OUTPUT_POSTS <username> <N>\n"
                     << "  LIST_FRIENDS <username>\n"
                     << "  SUGGEST_FRIENDS <username> <N>\n"
                     << "  DEGREE_OF_SEPARATION <username1> <username2>\n"
                     << "  HELP\n"
                     << "  EXIT\n";
                cout << endl;
                continue;
            }
            
            // EXIT: terminate program
            if(command[0] == "EXIT"){
                cout << "Exiting..." << endl;
                break;
            }

            // ADD_USER <username>: add a new user
            else if(command[0] == "ADD_USER"){
                if(command.size() != 2) throw invalid_argument("ADD_USER requires one argument: username");
                string username = to_lower(command[1]);
                if(data_base.find(username) != data_base.end()){
                    // If user already exists, print a friendly message and continue
                    cout << "Username already exists: " << username << "\n";
                    continue;
                }
                User* new_user = new User(username);
                data_base[username] = new_user;
                // Friendly confirmation to the user
                cout << "New user added: " << username << "\n";
            }
            
            // ADD_FRIEND <username1> <username2>: add each other as friends
            else if(command[0] == "ADD_FRIEND"){
                if(command.size() != 3) throw invalid_argument("ADD_FRIEND requires two arguments: username1 username2");
                string username1 = to_lower(command[1]);
                string username2 = to_lower(command[2]);
                if(data_base.find(username1) == data_base.end()) throw runtime_error("User " + username1 + " does not exist");
                if(data_base.find(username2) == data_base.end()) throw runtime_error("User " + username2 + " does not exist");
                if(username1 == username2) throw invalid_argument("A user cannot befriend themselves");
                data_base[username1]->addFriend(data_base[username2]);
                data_base[username2]->addFriend(data_base[username1]);
                cout << "Friendship created between " << username1 << " and " << username2 << "\n";
            }

            // ADD_POST <username> <post_content>: add a post for the user
            else if(command[0] == "ADD_POST"){
                if(command.size() != 3) throw invalid_argument("ADD_POST requires exactly two arguments: username post_content");
                string username = to_lower(command[1]);
                string post_content = command[2];
                if(data_base.find(username) == data_base.end()) throw runtime_error("User " + username + " does not exist");
                data_base[username]->addPost(post_content);
                cout << "Post added for " << username << ": \"" << post_content << "\"\n";
            }

            // OUTPUT_POSTS <username> <N>: output N posts of the user in post-order
            else if(command[0] == "OUTPUT_POSTS"){
                if(command.size() != 3) throw invalid_argument("OUTPUT_POSTS requires exactly two arguments: username N");
                string username = to_lower(command[1]);
                int N = stoi(command[2]);
                if(data_base.find(username) == data_base.end()) throw runtime_error("User " + username + " does not exist");
                data_base[username]->getPosts(N);
            }

            // LIST_FRIENDS <username>: list all friends of the user
            else if(command[0] == "LIST_FRIENDS"){
                if(command.size() != 2) throw invalid_argument("LIST_FRIENDS requires exactly one argument: username");
                string username = to_lower(command[1]);
                if(data_base.find(username) == data_base.end()) throw runtime_error("User " + username + " does not exist");
                data_base[username]->cout_Friends();
            }


            else if(command[0] == "SUGGEST_FRIENDS"){
                // Expect: SUGGEST_FRIENDS <username> <N>
                if(command.size() == 1) throw invalid_argument("SUGGEST_FRIENDS requires exactly two arguments: username N");
                string username = to_lower(command[1]);
                if(data_base.find(username) == data_base.end()) throw runtime_error("User " + username + " does not exist");
                int N;
                if(command.size() == 2)N = INT_MAX;
                else N = stoi(command[2]);
                if(N <= 0) {
                    // per spec: If N is 0, output nothing
                    continue;
                }

                User* me = data_base[username];

                unordered_set<User*> my_friends = me->getFriends();

                unordered_map<string, pair<int, User*>> candidates;
                
                for(User* f : my_friends){
                    for(User* fof : f->getFriends()){
                        if(fof == me) continue; // skip self
                        if(my_friends.find(fof) != my_friends.end()) continue; // skip existing friends
                        string cand_name = fof->getName();
                        if(candidates.find(cand_name) == candidates.end()) candidates[cand_name] = {0, fof};
                        candidates[cand_name].first += 1;
                    }
                }

                // Sort candidates by mutual count (desc), then username (asc)
                vector<pair<string, pair<int, User*>>> list;
                for(auto &kv : candidates) list.push_back(kv);
                sort(list.begin(), list.end(), [](const auto &a, const auto &b){
                    if(a.second.first != b.second.first) return a.second.first > b.second.first;
                    return a.first < b.first;
                });

                // Print up to N suggestions
                cout << "Suggested friends for " << username << ":\n";
                int printed = 0;

                for(auto &entry : list){
                    if(printed >= N) break;
                    cout << "    " << entry.first << " (" << entry.second.first << " mutual friends)\n";
                    printed++;
                }
            }

            else if(command[0] == "DEGREE_OF_SEPARATION" || command[0] == "DEGREES_OF_SEPARATION"){
                // Expect: DEGREE_OF_SEPARATION <username1> <username2>
                // Accept both singular and plural forms (DEGREE_OF_SEPARATION or DEGREES_OF_SEPARATION)
                if(command.size() != 3) throw invalid_argument("DEGREE_OF_SEPARATION requires exactly two arguments: username1 username2");
                string u1 = to_lower(command[1]);
                string u2 = to_lower(command[2]);
                if(data_base.find(u1) == data_base.end()) throw runtime_error("User " + u1 + " does not exist");
                if(data_base.find(u2) == data_base.end()) throw runtime_error("User " + u2 + " does not exist");

                User* start = data_base[u1];
                User* target = data_base[u2];

                if(start == target){
                    // Spec expects numeric report: 0
                    cout << 0 << endl;
                    continue;
                }

                // BFS to compute shortest path (degree of separation).
                queue<User*> q;
                unordered_map<User*, int> dist;
                unordered_set<User*> vis;
                q.push(start);
                dist[start] = 0;
                vis.insert(start);
                int answer = -1;
                while(!q.empty()){
                    User* cur = q.front(); q.pop();
                    int d = dist[cur];
                    for(User* fr : cur->getFriends()){
                        if(vis.find(fr) != vis.end()) continue;
                        vis.insert(fr);
                        dist[fr] = d + 1;
                        if(fr == target){ answer = d + 1; break; }
                        q.push(fr);
                    }
                    if(answer != -1) break;
                }

                // Per spec: report the length (integer). If no path exists, report -1.
                if (answer != -1){
                    cout << "Degree of separation between " << u1 << " and " << u2 << ": " << answer << endl;
                } else {
                    cout << "Degree of separation between " << u1 << " and " << u2 << ": -1 (no path exists)" << endl;
                }
            }

            else{
                throw invalid_argument("Unknown command: " + command[0]);
            }

        } catch(const exception& e) {
                cerr << "Error: " << e.what() << endl;
                cout << endl;
            }
    }
    return 0;
}