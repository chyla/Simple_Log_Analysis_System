/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_FANN_FANN_WRAPPER_H
#define LIBRARY_FANN_FANN_WRAPPER_H

#include "detail/fann_interface.h"
#include "detail/fann_wrapper_interface.h"

namespace library
{

namespace fann
{

class FannWrapper;
typedef std::shared_ptr<FannWrapper> FannWrapperPtr;

class FannWrapper : public detail::FannWrapperInterface {
 public:
  virtual ~FannWrapper() = default;

  static FannWrapperPtr Create();

  struct fann* CreateStandard(unsigned num_layers, unsigned num_input,
                              unsigned num_neurons_hidden, unsigned num_output) override;

  void SetActivationFunctionHidden(struct fann *ann,
                                   enum fann_activationfunc_enum activation_function) override;

  void SetActivationFunctionOutput(struct fann *ann,
                                   enum fann_activationfunc_enum activation_function) override;

  void TrainOnFile(struct fann *ann,
                   const std::string &filename,
                   unsigned int max_epochs,
                   unsigned int epochs_between_reports,
                   float desired_error) override;

  struct fann* CreateFromFile(const std::string &configuration_file) override;

  int Save(struct fann *ann, const std::string &configuration_file) override;

  fann_type* Run(struct fann *ann, fann_type *input) override;

  void Destroy(struct fann *ann) override;

 private:
  FannWrapper(detail::FannInterfacePtr fann_interface);

  detail::FannInterfacePtr fann_interface_;
};

}

}

#endif /* LIBRARY_FANN_FANN_WRAPPER_H */
