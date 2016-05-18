#include "command_receiver.h"

#include <boost/log/trivial.hpp>

#include "exception/detail/connection_is_already_open_exception.h"
#include "exception/detail/port_is_closed_exception.h"

using namespace std;

namespace web
{

CommandReceiverPtr CommandReceiver::Create(detail::CommandExecutorInterfacePtr command_executor,
                                           network::detail::NetworkInterfacePtr network) {
  auto p = CommandReceiverPtr(new CommandReceiver(command_executor, network));
  return p;
}

void CommandReceiver::OpenPort(const string &address, const int port) {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::OpenPort: Function call";
  if (socket_ != -1) {
    BOOST_LOG_TRIVIAL(error) << "web::CommandReceiver::OpenPort: Connection is already open (double OpenPort call?)";
    throw exception::detail::ConnectionIsAlreadyOpenException();
  }

  socket_ = network_->OpenIpv4Socket(address, port);
}

void CommandReceiver::ClosePort() {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::ClosePort: Function call";
  if (IsPortOpen() == false) {
    BOOST_LOG_TRIVIAL(error) << "web::CommandReceiver::ClosePort: Port is closed (missing OpenPort call?)";
    throw web::exception::detail::PortIsClosedException();
  }

  network_->Close(socket_);
  socket_ = -1;
}

bool CommandReceiver::IsPortOpen() const {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::IsPortOpen: Function call";
  return socket_ != -1;
}

void CommandReceiver::StartListen() {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::StartListen: Function call";
  if (IsPortOpen() == false) {
    BOOST_LOG_TRIVIAL(error) << "web::CommandReceiver::StartListen: Port is closed (missing OpenPort call?)";
    throw web::exception::detail::PortIsClosedException();
  }

  network::WaitStatus wait_status;
  network::ConnectionData client_connection;
  is_listen_ = true;
  while (is_listen_) {
    wait_status = network_->WaitForData(socket_, 3);

    if (wait_status != network::WaitStatus::NEW_DATA) {
      BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::StartListen: New data on socket";
      client_connection = network_->Accept(socket_);

      ReadAndExecuteCommand(client_connection.socket);

      network_->Close(client_connection.socket);
    }
    else {
      BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::StartListen: Timedout";
    }
  }
}

void CommandReceiver::StopListen() {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::StopListen: Function call";
  is_listen_ = false;
}

bool CommandReceiver::IsListen() const {
  return is_listen_;
}

CommandReceiver::CommandReceiver(detail::CommandExecutorInterfacePtr command_executor,
                                 network::detail::NetworkInterfacePtr network)
: command_executor_(command_executor),
network_(network),
socket_(-1),
is_listen_(false) {
}

void CommandReceiver::ReadAndExecuteCommand(int client_socket) {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::ReadAndExecuteCommand: Function call";
  network::WaitStatus wait_status;
  wait_status = network_->WaitForData(client_socket, 3);

  string command_text, command_result;
  if (wait_status == network::WaitStatus::NEW_DATA) {
    BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::ReadAndExecuteCommand: New data on socket";
    command_text = network_->ReceiveText(client_socket);

    command_result = command_executor_->Execute(command_text);

    network_->SendText(client_socket, command_result);
  }
  else {
    BOOST_LOG_TRIVIAL(debug) << "web::CommandReceiver::ReadAndExecuteCommand: Timedout";
  }
}

}
