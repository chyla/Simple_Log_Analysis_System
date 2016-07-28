/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "fann_wrapper.h"
#include "fann.h"

#include <boost/log/trivial.hpp>

namespace library
{

namespace fann
{

FannWrapperPtr FannWrapper::Create() {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannWrapper::Create: Function call";

  auto fann = Fann::Create();
  return FannWrapperPtr(new FannWrapper(fann));
}

struct fann* FannWrapper::CreateStandard(unsigned num_layers, unsigned num_input,
                                         unsigned num_neurons_hidden, unsigned num_output) {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannWrapper::CreateStandard: Function call";

  return fann_interface_->CreateStandard(num_layers, num_input, num_neurons_hidden, num_output);
}

void FannWrapper::SetActivationFunctionHidden(struct fann *ann,
                                              enum fann_activationfunc_enum activation_function) {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannWrapper::SetActivationFunctionHidden: Function call";

  fann_interface_->SetActivationFunctionHidden(ann, activation_function);
}

void FannWrapper::SetActivationFunctionOutput(struct fann *ann,
                                              enum fann_activationfunc_enum activation_function) {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannWrapper::SetActivationFunctionOutput: Function call";

  fann_interface_->SetActivationFunctionOutput(ann, activation_function);
}

void FannWrapper::TrainOnFile(struct fann *ann,
                              const std::string &filename,
                              unsigned int max_epochs,
                              unsigned int epochs_between_reports,
                              float desired_error) {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannWrapper::TrainOnFile: Function call";

  fann_interface_->TrainOnFile(ann, filename.c_str(), max_epochs, epochs_between_reports, desired_error);
}

struct fann* FannWrapper::CreateFromFile(const std::string &configuration_file) {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannWrapper::CreateFromFile: Function call";

  return fann_interface_->CreateFromFile(configuration_file.c_str());
}

int FannWrapper::Save(struct fann *ann, const std::string &configuration_file) {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannWrapper::Save: Function call";

  return fann_interface_->Save(ann, configuration_file.c_str());
}

fann_type* FannWrapper::Run(struct fann *ann, fann_type *input) {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannWrapper::Run: Function call";

  return fann_interface_->Run(ann, input);
}

void FannWrapper::Destroy(struct fann *ann) {
  BOOST_LOG_TRIVIAL(debug) << "library::fann::FannWrapper::Destroy: Function call";

  fann_interface_->Destroy(ann);
}

FannWrapper::FannWrapper(detail::FannInterfacePtr fann_interface) :
fann_interface_(fann_interface) {
}

}

}
