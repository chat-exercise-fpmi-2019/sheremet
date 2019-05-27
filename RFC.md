# Chat Prototol

This document describes network protocol for "client-server chat" educational exercise.

The chat consists of server and client. Client can issue three types of commands:

* Set username
* Send message
* Send broadcast message

Server can send three types of responses to client.

* Incoming message
* Failure reply for a client command
* Info message

## Connection

Clients establish a TCP (see RFC 793) connection with a server and communicate with text messages.

All text MUST be encoded as UTF-8 (RFC 3629) for network transmission.

Clients MAY NOT send messages until they set their usernames.

Clients and servers MAY do anything, including dropping the connection, after recieving a message badly formed according to this RFC.

Messages are fixed-size.

## Messages send from client to server

### UN

The UN message is being sent by clients to set their usernames and MUST be of form:

* string `UN` in upper case,
* followed by the username represented by a string of upper or lower case letters and numbers, with length of exactly 30 characters. If the username is not that long, it is followed by underscores to reach that exact length.

Example, as a C string literal: "UNusernameofnewuser_____________"

Servers SHOULD use the usernames provided by clients this way when retransmitting their messages to other clients.

If a user with the specified username already exists, servers MUST accept the message as well-formed and SHOULD reply to the sender with an appropriate IN message, while ignoring the new username.

### MS

The MS message is being sent by clients and MUST be of form:

* string `MS` in upper case;
* followed by the recipient username represented by a string of upper or lower case letters and numbers, with length of exactly 30 characters. If the username is not that long, it is followed by underscores to reach that exact length.
* followed by the message body with the length of exactly 32 characters. If the message is not that long, it is followed by the underscores.

Example, as a C string literal: "MSusernameofolduser_____________Hey! How's it going?____________"

Servers SHOULD forward the message to the specified recipient using the SM command.

Servers MUST accept empty messages(with body full of underscores) as well-formed.
Servers MUST accept messages to recipients that cannot be found as well-formed and in this case SHOULD reply to the sender with an appropriate IN message.

### BC

The BC message is being sent by clients and MUST be of form:

* string `BC` in upper case;
* followed by the message body with the length of exactly 30 characters. If the message is not that long, it is followed by the underscores.

Example, as a C string literal: "BCHey! How's it going?__________"

Servers SHOULD forward the message to all other users using the SM command.

Servers MUST accept empty messages(with body full of underscores) as well-formed.

## Messages from server to client

### SM

The SM message is sent by servers and MUST be of form:

* string `SM` in upper case;
* followed by the sender username represented by a string of upper or lower case letters and numbers, with length of exactly 30 characters. If the username is not that long, it is followed by underscores to reach that exact length.
* followed by the message body with the length of exactly 32 characters. If the message is not that long, it is followed by the underscores.

Example, as a C string literal: "SMusernameofnewuser_____________Hey! How's it going?____________"

### IN

The IN message is sent by servers and MUST be of form:

* string `IN` in upper case;
* followed by the message body with the length of exactly 62 characters. If the message is not that long, it is followed by the underscores.

Example, as a C string literal: "INuser usernameofnewuser is here!_______________________________"

Servers SHOULD send IN messages to describe connects and disconnects of users, errors with delivery of previous messages, and other service information.
