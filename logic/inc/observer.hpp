#pragma once
#include <list>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

class Subject {
    std::list<Observer *> list;
public:
    virtual ~Subject() = default;
    virtual void subscribe(Observer *ob) = 0;
    virtual void unsubscribe(Observer *ob) = 0;
    virtual void notify() = 0;
};
