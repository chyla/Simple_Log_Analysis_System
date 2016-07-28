/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBRARY_FANN_FANN_H
#define LIBRARY_FANN_FANN_H

#include "detail/fann_interface.h"

namespace library
{

namespace fann
{

class Fann;
typedef std::shared_ptr<Fann> FannPtr;

class Fann : public detail::FannInterface {
 public:
  virtual ~Fann() = default;

  static FannPtr Create();

  struct fann* CreateStandard(unsigned num_layers, unsigned num_input,
                              unsigned num_neurons_hidden, unsigned num_output) override;

  void SetActivationFunctionHidden(struct fann *ann,
                                   enum fann_activationfunc_enum activation_function) override;

  void SetActivationFunctionOutput(struct fann *ann,
                                   enum fann_activationfunc_enum activation_function) override;

  void TrainOnFile(struct fann *ann,
                   const char *filename,
                   unsigned int max_epochs,
                   unsigned int epochs_between_reports,
                   float desired_error) override;

  struct fann* CreateFromFile(const char *configuration_file) override;

  int Save(struct fann *ann, const char *configuration_file) override;

  fann_type* Run(struct fann *ann, fann_type *input) override;

  void Destroy(struct fann *ann) override;

 private:
  Fann() = default;
};

}

}

#endif /* LIBRARY_FANN_FANN_H */
