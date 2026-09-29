# The Plan

I want this game to be playable from my website. The way it's currently written, that is not possible. It would probably be easiest to rewrite the entire thing in Python, but I like the C code.

So, maybe the C code transforms a bit to become a game "host". You can request the host to begin a game, and it does its normal initialization and hands you back a token. The token is the key to your instance of the game.

From there, all you do is exchange rounds by telling the host your token and who is on the seesaw, and the host returns the result of that try.

After three tries, the host lets you know the game is over. Only then can you request who the unbalanced one was.

I think the easiest possible user interface is that there are 6 spots on each side of the seesaw, and you click the spots until they contain the user you would like them to contain. Then you hit the test button.

## The Host

Each instance of the game needs to store

* The token for this game
* The standard weight
* The user with the unequal weight
* The unequal weight
* The number of turns remaining

The public interface looks like

```c
// Submit the 12 character seesaw layout and receive back an indicator of which side turns down
int SubmitTurn(int iToken, char* acSeesawLayout);

// Submit your token, and receive the identity of the unequal person
int RevealIslander(int iToken);

// Submit your token, and receive the weight of the unequal person
int RevealWeight (int iToken);
```

How do I get the host to persistently store the state of the games?

* Could do a text file, a binary file, or mmap a file.
* Could try SQLite of MySQL.

## The Client

Actually, I think I should have the client come up with a token, since this will be easier in Python. I have no idea how I would handle it in C. If the backend cannot accept the token or already has a game under that token, it can refuse.

All the client needs to be able to do is put together a 12 char string and submit it to the host.

## Game Flow

Client makes token
Client requests new game from host with token argument
Host responds with success, failure, other error
Client submits layout 1, receives result
Client submits layout 2, receives result
Client submits layout 3, receives result
Client requests answer, receives
Host terminates game/token combo when the answer is requested
