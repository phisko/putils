# [Module](Module.hpp)

One "client" of a [Mediator](Mediator.md). `Modules` communicate by sending each other `DataPackets` (which can be of any user-specified type).

During class definition, a `Module` indicates what types of `DataPackets` it would like to receive, and must specify a

```cpp
void handle(const DataPacket &);
```

function for each of those types.

### Members

##### Definition

```cpp
template<typename CRTP, typename ...DataPackets>
class Module;
```

Modules must indicate their sub-type (see `CRTP`), as well as the types of `DataPackets` it would like to receive.

##### Constructor

```cpp
Module(Mediator *m = nullptr);
```

The Mediator can optionally be specified at construction time in order to send `DataPackets` or run tasks during construction.

##### send

```cpp
void send(const T &data) const
```

Sends the `data` `DataPacket` to all subscribed modules.

##### sendTo

```cpp
void sendTo(const T &data, BaseModule *dest) const;
```

Sends the `data` `DataPacket` to the `dest` `Module`.

##### query

```cpp
template<typename Response>
Response query(auto &&q); // q: DataPacket containing a "putils::BaseModule *sender" field
```

Lets a `Module` send a "query" to others, through a `DataPacket` containg a `sender` field.

Other `Modules` can reply to the query by calling `sendTo` with the `sender` field of the `DataPacket` and a `Response` as parameters.

##### runTask

```cpp
void runTask(const std::function<void()> &f) const;
```

Runs `f` asynchronously in the `Mediator`.
