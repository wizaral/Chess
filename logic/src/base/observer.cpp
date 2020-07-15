#include "observer.hpp"

Subscriber::Subscriber(Publisher *publisher)
    : publisher_(publisher) {
    publisher_->subscribe(this);
}
