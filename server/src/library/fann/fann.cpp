/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "fann.h"

namespace library
{

namespace fann
{

FannPtr Fann::Create() {
  return FannPtr(new Fann());
}

struct fann* Fann::CreateStandard(unsigned num_layers, unsigned num_input,
                                  unsigned num_neurons_hidden, unsigned num_output) {
  return fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
}

void Fann::SetActivationFunctionHidden(struct fann *ann,
                                       enum fann_activationfunc_enum activation_function) {
  fann_set_activation_function_hidden(ann, activation_function);
}

void Fann::SetActivationFunctionOutput(struct fann *ann,
                                       enum fann_activationfunc_enum activation_function) {
  fann_set_activation_function_output(ann, activation_function);
}

void Fann::TrainOnFile(struct fann *ann,
                       const char *filename,
                       unsigned int max_epochs,
                       unsigned int epochs_between_reports,
                       float desired_error) {
  fann_train_on_file(ann, filename, max_epochs, epochs_between_reports, desired_error);
}

struct fann* Fann::CreateFromFile(const char *configuration_file) {
  return fann_create_from_file(configuration_file);
}

int Fann::Save(struct fann *ann, const char *configuration_file) {
  return fann_save(ann, configuration_file);
}

fann_type* Fann::Run(struct fann *ann, fann_type *input) {
  return fann_run(ann, input);
}

void Fann::Destroy(struct fann *ann) {
  fann_destroy(ann);
}

}

}
