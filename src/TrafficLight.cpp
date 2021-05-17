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
  std::lock_guard<std::mutex> uLock(_mutex);

  // Make a trafficlight phase duration between 4 and 6 seconds
  // and switch to new state once it's done
  long cycleDuration =
      4000 + rand() % 2000; // duration of a single simulation cycle in ms
  std::chrono::time_point<std::chrono::system_clock> lastUpdate;
  lastUpdate = std::chrono::system_clock::now();

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    long timeSinceLastUpdate =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - lastUpdate)
            .count();

    if (timeSinceLastUpdate >= cycleDuration) {
      _currentPhase = _currentPhase == TrafficLightPhase::red
                          ? TrafficLightPhase::green
                          : TrafficLightPhase::red;

      _condition.notify_one();
      _messageQueue.send(std::move(_currentPhase));
      lastUpdate = std::chrono::system_clock::now();
    }
  }
}
