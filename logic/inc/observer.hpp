#pragma once
#include <list>

class Publisher;

class Subscriber {
protected:
    Publisher *publisher_;
public:
    Subscriber(Publisher *publisher)
        : publisher_(publisher) {
        publisher_->subscribe(this);
    }

    virtual ~Subscriber() = default;
    virtual void update() = 0;
};

class Publisher {
    std::list<Subscriber *> subs_;
public:
    virtual ~Publisher() = default;
    virtual void subscribe(Subscriber *sub) = 0;
    virtual void unsubscribe(Subscriber *sub) = 0;
    virtual void notify() = 0;
};
