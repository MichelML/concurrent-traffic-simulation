#include "TrafficLight.h"
#include <iostream>
#include <random>

/* Implementation of class "MessageQueue" */

/*
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and
_condition.wait()
    // to wait for and receive new messages and pull them from the queue using
move semantics.
    // The received object should then be returned by the receive function.
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms
std::lock_guard<std::mutex>
    // as well as _condition.notify_one() to add a new message to the queue and
afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() { _currentPhase = TrafficLightPhase::red; }

void TrafficLight::waitForGreen() {
  // FP.5b : add the implementation of the method waitForGreen, in which an
  // infinite while-loop
  // runs and repeatedly calls the receive function on the message queue.
  // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

void TrafficLight::simulate() {
  // FP.2b : Finally, the private method „cycleThroughPhases“ should be
  // started in a thread when the public method „simulate“ is called. To do
  // this, use the thread queue in the base class.
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
  }
}
