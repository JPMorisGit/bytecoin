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

#pragma once

#include <list>
#include <ostream>
#include <unordered_set>

#include <boost/uuid/uuid.hpp>
#include "Common/StringTools.h"
#include "crypto/hash.h"

namespace CryptoNote {

struct cryptonote_connection_context {
  boost::uuids::uuid m_connection_id;
  uint32_t m_remote_ip = 0;
  uint32_t m_remote_port = 0;
  bool m_is_income = false;
  time_t m_started = 0;

  enum state {
    state_befor_handshake = 0, //default state
    state_synchronizing,
    state_idle,
    state_normal,
    state_sync_required,
    state_shutdown
  };

  state m_state = state_befor_handshake;
  std::list<crypto::hash> m_needed_objects;
  std::unordered_set<crypto::hash> m_requested_objects;
  uint64_t m_remote_blockchain_height = 0;
  uint64_t m_last_response_height = 0;
};

inline std::string get_protocol_state_string(cryptonote_connection_context::state s) {
  switch (s)  {
  case cryptonote_connection_context::state_befor_handshake:
    return "state_befor_handshake";
  case cryptonote_connection_context::state_synchronizing:
    return "state_synchronizing";
  case cryptonote_connection_context::state_idle:
    return "state_idle";
  case cryptonote_connection_context::state_normal:
    return "state_normal";
  case cryptonote_connection_context::state_sync_required:
    return "state_sync_required";
  case cryptonote_connection_context::state_shutdown:
    return "state_shutdown";
  default:
    return "unknown";
  }
}

}

namespace std {
inline std::ostream& operator << (std::ostream& s, const CryptoNote::cryptonote_connection_context& context) {
  return s << "[" << Common::ipAddressToString(context.m_remote_ip) << ":" << 
    context.m_remote_port << (context.m_is_income ? " INC" : " OUT") << "] ";
}
}
