------------------------------------------------------------
Social Network CLI — README
------------------------------------------------------------

Author: Manya Jain
Project: simple in-memory social-network CLI

------------------------------------------------------------
What this program does
------------------------------------------------------------
This is a small command-line program that models a simple in-memory social
network. It supports adding users, creating friendships, posting short
messages, listing friends, suggesting friends (friends-of-friends ranked by
mutual friends) and computing degrees of separation between two users.

All data is kept in memory while the program runs; there is no persistent
storage. The program reads commands from stdin (interactive or redirected
from a file) and prints results to stdout. Errors are printed to stderr in the
format: Error: <message>

------------------------------------------------------------
How to compile and run
------------------------------------------------------------

Windows (using MSYS2 or g++ on PATH):

1. Open a terminal in the `long_assignment_col106` folder.
2. Run the batch file:

   compile.bat

   This will build `long_ass.exe` in the same folder.

3. Run the program interactively:

   long_ass.exe

   Or run with an input file:

   long_ass.exe < test_in.txt > test_out.txt

Linux / macOS (bash):

1. Open a terminal in the `long_assignment_col106` folder.
2. Run the script:

   ./compile.sh

   This will produce `long_ass`.

3. Run the program:

   ./long_ass

   Or with a file:

   ./long_ass < test_in.txt > test_out.txt

------------------------------------------------------------
Commands (one per line)
------------------------------------------------------------

HELP
    - Prints the list of available commands.

EXIT
    - Exits the program. Prints "Exiting...".

ADD_USER <username>
    - Add a new user. Usernames are normalized to lowercase.
    - Output: New user added: <username>

ADD_FRIEND <username1> <username2>
    - Make two users friends (bidirectional).
    - Output: Friendship created between <username1> and <username2>

ADD_POST <username> <post_content>
    - Add a post for the given user. Post content may include spaces.
    - Output: Post added for <username>: "<post_content>"

OUTPUT_POSTS <username> <N>
    - Print up to N posts from <username> (reverse-inorder traversal order).

LIST_FRIENDS <username>
    - List all direct friends of the user.

SUGGEST_FRIENDS <username> <N>
    - Recommend up to N friends-of-friends (not already friends). Ranking:
      number of mutual friends (descending). Ties broken alphabetically.

DEGREE_OF_SEPARATION <username1> <username2>
    - Print the length of the shortest friendship path between the users.
      Prints a friendly message and -1 if no path exists.

------------------------------------------------------------
Examples
------------------------------------------------------------

Example input (lines):

ADD_USER alice
ADD_USER bob
ADD_USER carol
ADD_FRIEND alice bob
ADD_FRIEND bob carol
SUGGEST_FRIENDS alice 5
DEGREE_OF_SEPARATION alice carol
EXIT

Example output (excerpt):

New user added: alice
New user added: bob
New user added: carol
Friendship created between alice and bob
Friendship created between bob and carol
Suggested friends for alice:
    carol
Degree of separation between alice and carol is 2

------------------------------------------------------------
Internal data structures
------------------------------------------------------------

- User: stores username, an AVL tree of posts (`posts.hpp`), and a set of
        pointers to friend `User` objects.
- AVL tree: used for storing posts and returning up to N posts via a
            reverse-inorder traversal.
- Friend graph: represented by `set<User*>` inside each `User`.

------------------------------------------------------------
Notes and limitations
------------------------------------------------------------

- The program keeps everything in memory; exiting the program clears all
  data.
- Usernames are normalized to lowercase for comparisons.
- Commands are space-separated; the parser keeps the remainder after the
  first two tokens as a single token (to allow post content with spaces).
- If you want quoted-argument parsing (e.g., support quoted usernames or
  other more complex parsing), I can add a more robust parser.

Contact: cs1240351@iitd.ac.in

------------------------------------------------------------