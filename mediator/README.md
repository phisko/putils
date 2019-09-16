# Mediator

A type-safe and self-documenting implementation of the Mediator pattern.

A [Mediator](Mediator.md) object manages [Modules](Module.md) and lets them communicate through user-defined (or pre-existing) `DataPacket` types.

### Classes

* [Mediator](Mediator.md)
* [Module](Module.md)

### Example

```cpp
int main()
{
    struct ExampleQuery
    {
        std::string data;
        BaseModule *sender;
    };

    struct LogModule : putils::Module<LogModule, std::string, ExampleQuery> // will receive std::strings and ExampleQueries
    {
        void handle(const std::string &packet) const { std::cout << "Received " << packet << std::endl; }

        void handle(const ExampleQuery &packet) const { sendTo(42, packet.sender); }
    };

    struct SenderModule : putils::Module<SenderModule> // will not receive anything
    {
        void doWork() const { send(std::string("hi there!")); }

        void doQuery() { std::cout << "Query result is " << query<int>(ExampleQuery{ "data", nullptr }) << std::endl; }
    };

    Mediator mediator;
    SenderModule sender;
    LogModule log;

    mediator.addModule(sender);
    mediator.addModule(log);

    sender.doWork();
    sender.doQuery();
}
```
