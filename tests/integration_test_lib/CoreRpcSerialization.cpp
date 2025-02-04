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

#include "CoreRpcSerialization.h"

namespace CryptoNote {

void serialize(COMMAND_RPC_START_MINING::request& value, const std::string& name, CryptoNote::ISerializer& serializer) {
  serializer.beginObject(name);
  serializer(value.miner_address, "miner_address");
  serializer(value.threads_count, "threads_count");
  serializer.endObject();
}

void serialize(COMMAND_RPC_START_MINING::response& value, const std::string& name, CryptoNote::ISerializer& serializer) {
  serializer.beginObject(name);
  serializer(value.status, "status");
  serializer.endObject();
}

void serialize(COMMAND_RPC_STOP_MINING::request& value, const std::string& name, CryptoNote::ISerializer& serializer) {
  serializer.beginObject(name);
  serializer.endObject();
}

void serialize(COMMAND_RPC_STOP_MINING::response& value, const std::string& name, CryptoNote::ISerializer& serializer) {
  serializer.beginObject(name);
  serializer(value.status, "status");
  serializer.endObject();
}

void serialize(COMMAND_RPC_STOP_DAEMON::request& value, const std::string& name, CryptoNote::ISerializer& serializer) {
  serializer.beginObject(name);
  serializer.endObject();
}

void serialize(COMMAND_RPC_STOP_DAEMON::response& value, const std::string& name, CryptoNote::ISerializer& serializer) {
  serializer.beginObject(name);
  serializer(value.status, "status");
  serializer.endObject();
}

} //namespace CryptoNote
