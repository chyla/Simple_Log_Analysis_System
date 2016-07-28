/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_FANN_DETAIL_FANN_WRAPPER_INTERFACE_H
#define LIBRARY_FANN_DETAIL_FANN_WRAPPER_INTERFACE_H

#include "fann_interface.h"

#include <memory>
#include <string>

namespace library
{

namespace fann
{

namespace detail
{

class FannWrapperInterface {
 public:
  virtual ~FannWrapperInterface() = default;

  virtual struct fann* CreateStandard(unsigned num_layers, unsigned num_input,
                                      unsigned num_neurons_hidden, unsigned num_output) = 0;

  virtual void SetActivationFunctionHidden(struct fann *ann,
                                           enum fann_activationfunc_enum activation_function) = 0;

  virtual void SetActivationFunctionOutput(struct fann *ann,
                                           enum fann_activationfunc_enum activation_function) = 0;

  virtual void TrainOnFile(struct fann *ann,
                           const std::string &filename,
                           unsigned int max_epochs,
                           unsigned int epochs_between_reports,
                           float desired_error) = 0;

  virtual struct fann* CreateFromFile(const std::string &configuration_file) = 0;

  virtual int Save(struct fann *ann, const std::string &configuration_file) = 0;

  virtual fann_type* Run(struct fann *ann, fann_type *input) = 0;

  virtual void Destroy(struct fann *ann) = 0;
};

typedef std::shared_ptr<FannWrapperInterface> FannWrapperInterfacePtr;

}

}

}

#endif /* LIBRARY_FANN_DETAIL_FANN_WRAPPER_INTERFACE_H */
