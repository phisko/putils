// gtest
#include <gtest/gtest.h>

// mediator
#include "mediator/Mediator.hpp"
#include "mediator/Module.hpp"

struct SenderModule : putils::Module<SenderModule> {
    using Module::Module;

    void doWork() noexcept {
        send(42);
    }
};

struct ReceiverModule : putils::Module<ReceiverModule, int> {
    using Module::Module;

    int received = 0;

    void handle(int i) noexcept {
        received = i;
    }
};

TEST(Mediator, sendReceive) {
    putils::Mediator mediator;
    SenderModule sender(&mediator);
    ReceiverModule receiver(&mediator);
    sender.doWork();

    EXPECT_EQ(receiver.received, 42);
}

TEST(Mediator, multipleReceivers) {
    putils::Mediator mediator;
    SenderModule sender(&mediator);
    ReceiverModule receiver(&mediator);
    ReceiverModule receiver2(&mediator);
    sender.doWork();

    EXPECT_EQ(receiver.received, 42);
    EXPECT_EQ(receiver2.received, 42);
}
