#include "TrafficLight.h"
#include <future>
#include <iostream>
#include <random>
#include <thread>

/* Implementation of class "MessageQueue" */

template <typename T> T MessageQueue<T>::receive() {
  // perform queue modification under the lock
  std::unique_lock<std::mutex> uLock(_mut);
  _cond.wait(uLock, [this] {
    return !_queue.empty();
  }); // pass unique lock to condition variable

  // remove last vector element from queue
  T msg = std::move(_queue.back());
  _queue.pop_back();

  return msg; // will not be copied due to return value optimization (RVO) in
              // C++
}

template <typename T> void MessageQueue<T>::send(T &&msg) {
  // wait one ms
  std::this_thread::sleep_for(std::chrono::milliseconds(1));

  // perform vector modification under the lock
  std::lock_guard<std::mutex> uLock(_mut);

  // add message to queue
  _queue.push_back(std::move(msg));
  _cond.notify_one(); // notify client after pushing new Vehicle into vector
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() { _currentPhase = TrafficLightPhase::red; }
TrafficLight::~TrafficLight() {}

void TrafficLight::waitForGreen() {
  // FP.5b : add the implementation of the method waitForGreen, in which an
  // infinite while-loop
  // runs and repeatedly calls the receive function on the message queue.
  // Once it receives TrafficLightPhase::green, the method returns.
  while (true) {
    if (_messageQueue.receive() == TrafficLightPhase::green) {
      return;
    }
  }
}

TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

void TrafficLight::simulate() {
  // FP.2b : Finally, the private method „cycleThroughPhases“ should be
  // started in a thread when the public method „simulate“ is called. To do
  // this, use the thread queue in the base class.
  threads.emplace_back(
      std::thread(&TrafficLight::cycleThroughPhases, std::move(this)));
}

void TrafficLight::cycleThroughPhases() {
  while (true) {
    // wait for 1 ms between cycle
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // perform work under the lock
    std::lock_guard<std::mutex> uLock(_mutex);

    // Cycle duration is a value between 4 and 6 seconds
    std::this_thread::sleep_for(
        std::chrono::milliseconds(std::rand() % (6000 - 4000 + 1)));

    // Switch to new state
    if (_currentPhase == TrafficLightPhase::red) {
      _currentPhase = TrafficLightPhase::green;
    } else {
      _currentPhase = TrafficLightPhase::red;
    }

    // notify state has changed
    _condition.notify_one();
    _messageQueue.send(std::move(_currentPhase));
  }
}
