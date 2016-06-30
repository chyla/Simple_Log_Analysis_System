/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef NOTIFIER_DETAIL_NOTIFIER_H
#define NOTIFIER_DETAIL_NOTIFIER_H

#include "detail/notifier_interface.h"
#include "detail/system_interface.h"

#include "src/mailer/detail/mailer_interface.h"
#include "src/program_options/type/options.h"

#include <memory>
#include <mutex>

namespace notifier
{

class Notifier;
typedef std::shared_ptr<Notifier> NotifierPtr;

class Notifier : public detail::NotifierInterface {
 public:
  virtual ~Notifier() = default;

  static NotifierPtr Create(const ::program_options::type::Options &options);
  static NotifierPtr Create(const ::program_options::type::Options &options,
                            ::mailer::detail::MailerInterfacePtr mailer,
                            detail::SystemInterfacePtr system);

  void Loop() override;
  void StopLoop() override;
  bool IsRunning() const override;

  void AddMessages(::notifier::type::NotifierMessages messages) override;

 private:
  Notifier(const ::program_options::type::Options &options,
           ::mailer::detail::MailerInterfacePtr mailer,
           detail::SystemInterfacePtr system);

  ::notifier::type::NotifierMessageInterfacePtr GetMessage();
  bool IsMessageAvailable();

  void SendMailNotification();

  const ::program_options::type::Options &options_;
  ::mailer::detail::MailerInterfacePtr mailer_;
  detail::SystemInterfacePtr system_;
  bool running_;

  ::notifier::type::NotifierMessages messages_;
  std::mutex messages_mutex_;
};

}

#endif /* NOTIFIER_H */
