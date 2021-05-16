#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "TrafficObject.h"
#include <condition_variable>
#include <deque>
#include <mutex>

// forward declarations to avoid include cycle
class Vehicle;

// FP.3 Define a class „MessageQueue“ which has the public methods send and
// receive. Send should take an rvalue reference of type TrafficLightPhase
// whereas receive should return this type. Also, the class should define an
// std::dequeue called _queue, which stores objects of type TrafficLightPhase.
// Also, there should be an std::condition_variable as well as an std::mutex as
// private members.

template <class T> class MessageQueue {
public:
  T receive();
  void send(T &&msg);

private:
  std::deque<T> _queue;
  std::condition_variable _cond;
  std::mutex _mut;
};

enum TrafficLightPhase { red, green };
class TrafficLight : public TrafficObject {
public:
  // constructor / desctructor
  TrafficLight();
  ~TrafficLight();

  // getters/setters
  TrafficLightPhase getCurrentPhase();

  // typical behaviour methods
  void waitForGreen();
  void simulate();

private:
  // typical behaviour methods
  void cycleThroughPhases();
  // FP.4b : create a private member of type MessageQueue for messages of type
  // TrafficLightPhase and use it within the infinite loop to push each new
  // TrafficLightPhase into it by calling send in conjunction with move
  // semantics.

  TrafficLightPhase _currentPhase;
  std::condition_variable _condition;
  std::mutex _mutex;
  MessageQueue<TrafficLightPhase> _messageQueue;
};

#endif