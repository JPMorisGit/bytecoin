// Copyright (c) 2012-2015, The CryptoNote developers, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Bytecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bytecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Bytecoin.  If not, see <http://www.gnu.org/licenses/>.

#include "Timer.h"
#include <cassert>
#include <stdexcept>
#include <string>

#include <sys/errno.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

#include "Dispatcher.h"
#include <System/InterruptedException.h>

namespace System {

Timer::Timer() : dispatcher(nullptr) {
}

Timer::Timer(Dispatcher& dispatcher) : dispatcher(&dispatcher), stopped(false), context(nullptr), timer(-1) {
}

Timer::Timer(Timer&& other) : dispatcher(other.dispatcher) {
  if (other.dispatcher != nullptr) {
    assert(other.context == nullptr);
    timer = other.timer;
    stopped = other.stopped;
    context = nullptr;
    other.dispatcher = nullptr;
  }
}

Timer::~Timer() {
  assert(dispatcher == nullptr || context == nullptr);
}

Timer& Timer::operator=(Timer&& other) {
  assert(dispatcher == nullptr || context == nullptr);
  dispatcher = other.dispatcher;
  if (other.dispatcher != nullptr) {
    assert(other.context == nullptr);
    timer = other.timer;
    stopped = other.stopped;
    context = nullptr;
    other.dispatcher = nullptr;
    other.timer = -1;
  }

  return *this;
}

void Timer::start() {
  assert(dispatcher != nullptr);
  assert(stopped);
  stopped = false;
}

void Timer::stop() {
  assert(dispatcher != nullptr);
  assert(!stopped);

  if (context != nullptr) {
    Dispatcher::OperationContext* timerContext = static_cast<Dispatcher::OperationContext*>(context);
    if (!timerContext->interrupted) {
      struct kevent event;
      EV_SET(&event, timer, EVFILT_TIMER, EV_DISABLE, 0, 0, NULL);

      if (kevent(dispatcher->getKqueue(), &event, 1, NULL, 0, NULL) == -1) {
        throw std::runtime_error("Timer::stop, kevent() failed, errno=" + std::to_string(errno));
      }

      dispatcher->pushContext(timerContext->context);
      timerContext->interrupted = true;
    }
  }

  stopped = true;
}

void Timer::sleep(std::chrono::milliseconds duration) {
  assert(dispatcher != nullptr);
  assert(context == nullptr);
  if (stopped) {
    throw InterruptedException();
  }

  Dispatcher::OperationContext timerContext;
  timerContext.context = dispatcher->getCurrentContext();
  timerContext.interrupted = false;
  timer = dispatcher->getTimer();

  struct kevent event;
  EV_SET(&event, timer, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, duration.count(), &timerContext);

  if (kevent(dispatcher->getKqueue(), &event, 1, NULL, 0, NULL) == -1) {
    throw std::runtime_error("Timer::stop, kevent() failed, errno=" + std::to_string(errno));
  }

  context = &timerContext;
  dispatcher->dispatch();
  assert(dispatcher != nullptr);
  assert(timerContext.context == dispatcher->getCurrentContext());
  assert(context == &timerContext);
  context = nullptr;
  timerContext.context = nullptr;
  dispatcher->pushTimer(timer);
  if (timerContext.interrupted) {
    throw InterruptedException();
  }
}

}
