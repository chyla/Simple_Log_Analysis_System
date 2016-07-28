/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_FANN_DETAIL_FANN_INTERFACE_H
#define LIBRARY_FANN_DETAIL_FANN_INTERFACE_H

#include <floatfann.h>
#include <fann.h>

#include <memory>

namespace library
{

namespace fann
{

namespace detail
{

class FannInterface {
 public:
  virtual ~FannInterface() = default;

  virtual struct fann* CreateStandard(unsigned num_layers, unsigned num_input,
                                      unsigned num_neurons_hidden, unsigned num_output) = 0;

  virtual void SetActivationFunctionHidden(struct fann *ann,
                                           enum fann_activationfunc_enum activation_function) = 0;

  virtual void SetActivationFunctionOutput(struct fann *ann,
                                           enum fann_activationfunc_enum activation_function) = 0;

  virtual void TrainOnFile(struct fann *ann,
                           const char *filename,
                           unsigned int max_epochs,
                           unsigned int epochs_between_reports,
                           float desired_error) = 0;

  virtual struct fann* CreateFromFile(const char *configuration_file) = 0;

  virtual int Save(struct fann *ann, const char *configuration_file) = 0;

  virtual fann_type* Run(struct fann *ann, fann_type *input) = 0;

  virtual void Destroy(struct fann *ann) = 0;
};

typedef std::shared_ptr<FannInterface> FannInterfacePtr;

}

}

}

#endif /* LIBRARY_FANN_DETAIL_FANN_INTERFACE_H */
