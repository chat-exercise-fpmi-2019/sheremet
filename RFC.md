# Chat Prototol

This document describes network protocol for "client-server chat" educational exercise.

The chat consist of server and client. Client can issue three types of commands:

* Set username
* Input password to authorize
* Send message

Server can send three types of responses to client.

* Incoming message
* Password request
* Failure reply for a client command
* Custom notification

## Connection

Clients establish a TCP (see RFC 793) connection with a server and communicate with text messages.

All text MUST be encoded as UTF-8 (RFC 3629) for network transmission.

Clients MAY NOT send messages until they are authorized.

Clients and servers MAY do anything, including dropping the connection, after recieving a message badly formed according to this RFC.

## Messages send from client to server

### USERNAME

The USERNAME message is being sent by clients and MUST be of form:

* string `USERNAME` in upper case,
* followed by exactly one normal space character (ASCII "SP"),
* followed by the username represented by a string of upper or lower case letters, numbers or underscores,
* followed by exactly one normal newline character (ASCII "LF").

Username length MUST be between 1 and 64 characters.

Example, as a C string literal: "USERNAME user1234\n"

Servers SHOULD use the usernames provided by clients this way when retransmitting their messages to other clients.

Server SHOULD request user's password by PASSWORDREQUEST message, for example "PASSWORDREQUEST user1234\n", or, if user name is incorrect, send client failure response

### PASSWORD

The PASSWORD message is being sent by clients and MUST be of form:

* string `PASSWORD` in upper case,
* followed by exactly one normal space character (ASCII "SP"),
* followed by the password represented by a string of upper or lower case letters, numbers or underscores,
* followed by exactly one normal newline character (ASCII "LF").

Password length MUST be between 8 and 16 characters.

Example, as a C string literal: "PASSWORD 12345678\n"

If the password is correct, server should react with corresponding INFO message, for example "INFO Welcome, user1234\n". Else server should send failure response

### SEND

The SEND message is being sent by clients and MUST be of form:

* string `SEND` in upper case;
* followed by exactly one normal space character;
* followed by the recipient username represented by a string of upper or lower case letters, numbers or underscores;
* followed by exactly one normal space character;
* followed by the message body length (LENGTH) in bytes, a nonnegative integer represented as a string of decimal digits without leading zeros;
* followed by exactly one normal newline character;
* followed by the message body represented as an exactly LENGTH bytes long sequence of arbitrary symbols;
* followed by exactly one normal newline character.

Example, as a C string literal: "SEND user123 17\nhi!\nhow are you?\n\n"

Servers SHOULD forward the message to the specified recipient using the `MESSAGE` command.

Servers MUST accept zero-body-length messages as well-formed.
Servers MUST accept messages to recipients that cannot be found as well-formed and in this case SHOULD reply to the sender with an appropriate INFO message.

## Messages from server to client

### MESSAGE

The MESSAGE message is sent by servers and MUST be of form:

* string `MESSAGE` in upper case;
* followed by exactly one normal space character;
* followed by the sender username represented by a string of upper or lower case letters, numbers or underscores;
* followed by exactly one normal space character;
* followed by the message body length (LENGTH) in bytes, a nonnegative integer represented as a string of decimal digits without leading zeros;
* followed by exactly one normal newline character;
* followed by the message body represented as an exactly LENGTH bytes long sequence of arbitrary symbols;
* followed by exactly one normal newline character.

Example, as a C string literal: "MESSAGE user122 17\nhi!\nhow are you?\n\n"

### PASSWORDREQUEST

The PASSWORDREQUEST message is sent by servers and MUST be of form:

* string `PASSWORDREQUEST` in upper case;
* followed by exactly one normal space character;
* followed by the username represented by a string of upper or lower case letters, numbers or underscores,
* followed by exactly one normal newline character (ASCII "LF").

Example, as a C string literal: "PASSWORDREQUEST user1234\n"

### INFO

The INFO message is sent by servers and MUST be of form:

* string `INFO` in upper case;
* followed by exactly one normal space character;
* followed by the message body length (LENGTH) in bytes, a nonnegative integer represented as a string of decimal digits without leading zeros;
* followed by exactly one normal newline character;
* followed by the message body represented as an exactly LENGTH bytes long sequence of arbitrary symbols;
* followed by exactly one normal newline character.

Example, as a C string literal: "INFO 28\nuser johnny entered the chat\n"

Servers SHOULD send INFO messages to describe connects and disconnects of users, errors with delivery of previous messages, and other service information.
