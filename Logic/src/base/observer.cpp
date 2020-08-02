#include "observer.hpp"

Subscriber::Subscriber(Publisher *publisher)
    : publisher_(publisher) {
    publisher_->subscribe(this);
}

Subscriber::~Subscriber() {
    publisher_->unsubscribe(this);
}

Publisher::~Publisher() {
    for (auto i : subs_) {
        i->update(Subscriber::MessageType::Destroy);
    }
}
