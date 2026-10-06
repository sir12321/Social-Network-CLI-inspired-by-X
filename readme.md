# Social Network CLI (Inspired by X)

**Author:** Manya Jain
**Course:** COL106 Data Structures & Algorithms, IIT Delhi

An in-memory social network driven from the command line, written in C++17.
Users form an undirected friendship graph, each user's posts are stored in a
self-balancing AVL tree, and the graph supports friend suggestions and
degree-of-separation queries.

All data lives in memory for the duration of the run; there is no persistent
storage. Commands are read from stdin (interactively or from a file), results
go to stdout, and errors go to stderr as `Error: <message>`.

## Build and run

**Linux / macOS**

```bash
./compile.sh                      # builds ./long_ass
./long_ass                        # interactive
./long_ass < input.txt > out.txt  # batch
```

**Windows** (MSYS2 UCRT g++ if installed, otherwise `g++` on PATH)

```bat
compile.bat
long_ass.exe
long_ass.exe < input.txt > out.txt
```

## Commands

One command per line. Usernames are case-insensitive (normalized to lowercase).

| Command | Description |
| --- | --- |
| `ADD_USER <username>` | Create a new user. |
| `ADD_FRIEND <username1> <username2>` | Make two users friends (bidirectional). |
| `ADD_POST <username> <post_content>` | Add a post; the content may contain spaces. |
| `OUTPUT_POSTS <username> <N>` | Print the user's N most recent posts, newest first. `N = -1` prints all. |
| `LIST_FRIENDS <username>` | List the user's direct friends alphabetically. |
| `SUGGEST_FRIENDS <username> [N]` | Recommend up to N friends-of-friends who are not already friends, ranked by mutual-friend count (descending), ties broken alphabetically. Omitting N lists all; N ≤ 0 prints nothing. |
| `DEGREE_OF_SEPARATION <username1> <username2>` | Length of the shortest friendship path, or -1 if none exists. `DEGREES_OF_SEPARATION` is also accepted. |
| `HELP` | List the available commands. |
| `EXIT` | Quit. |

## Example

Input:

```
ADD_USER alice
ADD_USER bob
ADD_USER carol
ADD_FRIEND alice bob
ADD_FRIEND bob carol
ADD_POST alice Hello world
ADD_POST alice Second post
OUTPUT_POSTS alice 5
SUGGEST_FRIENDS alice 5
DEGREE_OF_SEPARATION alice carol
EXIT
```

Output:

```
New user added: alice
New user added: bob
New user added: carol
Friendship created between alice and bob
Friendship created between bob and carol
Post added for alice: "Hello world"
Post added for alice: "Second post"
Posts of alice :
    Post Content: Second post
    Post Content: Hello world
Suggested friends for alice:
    carol (1 mutual friends)
Degree of separation between alice and carol: 2
Exiting...
```

## Design

- **Users** (`user.hpp`): a username, an AVL tree of posts, and an
  `unordered_set<User*>` of friends. `ADD_FRIEND` inserts each user into the
  other's set, so the friendship graph is undirected.
- **Posts** (`posts.hpp`): an AVL tree keyed by an incrementing post id, kept
  balanced with single and double rotations. A reverse in-order traversal
  (right, root, left) visits posts newest first and stops after N.
- **Friend suggestions**: counts how many of the user's friends each
  friend-of-friend is connected to, then sorts by that mutual count.
- **Degree of separation**: breadth-first search from the first user,
  stopping as soon as the second user is reached.

## Limitations

- No persistence: all data is lost on exit.
- Arguments are space-separated; everything after the second token is kept as
  one argument (so post content can contain spaces), and quoted arguments are
  not supported.
