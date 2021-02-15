#include "observer.hpp"

Subscriber::Subscriber(Publisher *publisher)
: m_publisher(publisher) {
    if (m_publisher != nullptr) {
        m_publisher->subscribe(this);
    }
}

Subscriber::~Subscriber() {
    if (m_publisher != nullptr) {
        m_publisher->unsubscribe(this);
    }
}

Publisher::~Publisher() {
    for (auto i : m_subs) {
        i->update(Subscriber::MessageType::Destroy);
    }
}
