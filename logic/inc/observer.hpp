#pragma once
#include <list>

class Publisher;

class Subscriber {
protected:
    Publisher *m_publisher;

public:
    enum class MessageType {
        Notify,
        Destroy,
    };

    Subscriber(Publisher *publisher);
    virtual ~Subscriber();
    virtual bool update(MessageType type) = 0;
};

class Publisher {
protected:
    std::list<Subscriber *> m_subs;

public:
    virtual ~Publisher();
    virtual void subscribe(Subscriber *sub) = 0;
    virtual void unsubscribe(Subscriber *sub) = 0;
    virtual void notify() = 0;
};
