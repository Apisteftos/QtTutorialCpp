# 01 · Chat Application

A small chat that runs inside one process. A server, three clients and a few channels exchange text messages through an in-memory "network", so the example needs no sockets and no Qt.

## Patterns used

| Pattern | Where it appears in `main.cpp` |
|---|---|
| Layered | `Network` (transport) → `Chat` (server and client logic) → `Ui` (rendering text for the user) |
| Client-Server | Clients only talk to the `Server`; they never send to each other |
| Pub-Sub | The server keeps `channel → subscribers`; a `SAY` is published to everyone in the channel except the sender |
| Event-Driven | Every packet is an event in a queue; `Hub::run()` takes events one by one and calls the handler of the receiver |

## Sections in `main.cpp`

- LAYER 1: Network — `Hub` with `attach`, `send`, `run`
- LAYER 3: Ui — `split_word`, `render`
- LAYER 2: Chat — `Server` and `Client`
- EXAMPLE 1: Connect and join a channel
- EXAMPLE 2: Publish to a channel
- EXAMPLE 3: Only subscribers receive messages
- EXAMPLE 4: Leave a channel

## Protocol

One text line per packet, `COMMAND arguments`.

| Direction | Line | Meaning |
|---|---|---|
| client → server | `HELLO name` | register a name |
| client → server | `JOIN channel` / `LEAVE channel` | subscribe / unsubscribe |
| client → server | `SAY channel text` | publish a message (only allowed for subscribers) |
| server → client | `MSG channel sender text` | a published message |
| server → client | `INFO text` | information (welcome, joined, left) |
| server → client | `ERR text` | an error, for example saying something in a channel you are not in |

## Output

```text
=== Example 1: Connect and join a channel ===
  [alice] * welcome alice
  [bob] * welcome bob
  [carol] * welcome carol
  [alice] * you joined #general
  [alice] * bob joined #general
  [bob] * you joined #general

=== Example 2: Publish to a channel ===
  [bob] #general alice: hello everyone
  [alice] #general bob: hi alice

=== Example 3: Only subscribers receive messages ===
  [carol] * you joined #random
  [carol] * bob joined #random
  [bob] * you joined #random
  [alice] ! you are not in #random
  [bob] #random carol: welcome to #random, bob

=== Example 4: Leave a channel ===
  [alice] * bob left #general
  [bob] * you left #general
  [bob] ! you are not in #general
```

Messages are not delivered to their sender, which is why `alice` does not see her own text.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o chat
./chat
```

The `CMakeLists.txt` builds the same single file (no Qt needed).

## Ideas to extend it

- Replace `Network::Hub` with real TCP sockets; `Server` and `Client` would not need to change if the new class keeps `attach`, `send` and `run`.
- Keep a message history per channel and send it to a client when it joins.
- Add private messages (`DM name text`).
- Handle a client that disconnects without sending `LEAVE`.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 23 lines, identical on two runs.
- Clean under AddressSanitizer + UBSan.
- `cmake` configures the project.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- A full CMake build.

## Notes

The previous `main.cpp` of this folder was only an empty Qt skeleton (it started an event loop and did nothing). This version replaces it with a working example.
