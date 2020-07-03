#pragma once
#include <list>

class Observer {
public:
    virtual ~Observer() {}
    virtual void update() = 0;
};

class Subject {
    std::list<Observer *> list;
public:
    virtual ~Subject() {}
    virtual void subscribe(Observer *ob) = 0;
    virtual void unsubscribe(Observer *ob) = 0;
    virtual void notify() = 0;
};
