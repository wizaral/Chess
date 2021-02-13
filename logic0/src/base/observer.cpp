#include "observer.hpp"

Subscriber::Subscriber(Publisher *publisher)
    : publisher_(publisher) {
    if (publisher_ != nullptr) {
        publisher_->subscribe(this);
    }
}

Subscriber::~Subscriber() {
    if (publisher_ != nullptr) {
        publisher_->unsubscribe(this);
    }
}

Publisher::~Publisher() {
    for (auto i : subs_) {
        i->update(Subscriber::MessageType::Destroy);
    }
}
