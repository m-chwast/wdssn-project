/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef TENSORFLOW_LITE_MICRO_MICRO_MUTABLE_OP_RESOLVER_H_
#define TENSORFLOW_LITE_MICRO_MICRO_MUTABLE_OP_RESOLVER_H_

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/core/api/op_resolver.h"
#include "tensorflow/lite/micro/compatibility.h"
#include "tensorflow/lite/schema/schema_generated.h"

#ifndef TFLITE_REGISTRATIONS_MAX
#define TFLITE_REGISTRATIONS_MAX (128)
#endif

namespace tflite {

// Op versions discussed in this file are enumerated here:
// tensorflow/lite/tools/versioning/op_version.cc

inline int MicroOpResolverAnyVersion() { return 0; }

template <unsigned int tOpCount = TFLITE_REGISTRATIONS_MAX>
class MicroOpResolver : public OpResolver {
 public:
  explicit MicroOpResolver(ErrorReporter* error_reporter = nullptr)
      : error_reporter_(error_reporter) {}

  const TfLiteRegistration* FindOp(tflite::BuiltinOperator op,
                                   int version) const override {
    for (unsigned int i = 0; i < registrations_len_; ++i) {
      const TfLiteRegistration& registration = registrations_[i];
      if ((registration.builtin_code == op) &&
          (registration.version == MicroOpResolverAnyVersion() ||
           version == MicroOpResolverAnyVersion() ||
           registration.version == version ||
		   op == BuiltinOperator_TANH)) {
        return &registration;
      }
    }
    return nullptr;
  }

  const TfLiteRegistration* FindOp(const char* op, int version) const override {
    for (unsigned int i = 0; i < registrations_len_; ++i) {
      const TfLiteRegistration& registration = registrations_[i];
      if ((registration.builtin_code == BuiltinOperator_CUSTOM) &&
          (strcmp(registration.custom_name, op) == 0) &&
          (registration.version == MicroOpResolverAnyVersion() ||
           version == MicroOpResolverAnyVersion() ||
           registration.version == version)) {
        return &registration;
      }
    }
    return nullptr;
  }

  TfLiteStatus AddBuiltin(tflite::BuiltinOperator op,
                          TfLiteRegistration* registration, int version = 1) {
    if (registrations_len_ >= tOpCount) {
      if (error_reporter_) {
        TF_LITE_REPORT_ERROR(error_reporter_,
                             "Couldn't register builtin op #%d, resolver size "
                             "is too small (%d)",
                             op, tOpCount);
      }
      return kTfLiteError;
    }
    TfLiteRegistration* new_registration = &registrations_[registrations_len_];
    registrations_len_ += 1;

    *new_registration = *registration;
    new_registration->builtin_code = op;
    new_registration->version = version;

    return kTfLiteOk;
  }

  TfLiteStatus AddBuiltin(tflite::BuiltinOperator op,
                          TfLiteRegistration* registration, int min_version,
                          int max_version) {
    for (int version = min_version; version <= max_version; ++version) {
      TfLiteStatus add_status = AddBuiltin(op, registration, version);
      if (add_status != kTfLiteOk) {
        return add_status;
      }
    }
    return kTfLiteOk;
  }

  TfLiteStatus AddCustom(const char* name, TfLiteRegistration* registration,
                         int version = 1) {
    if (registrations_len_ >= tOpCount) {
      if (error_reporter_) {
        TF_LITE_REPORT_ERROR(
            error_reporter_,
            "Couldn't register custom op '%s', resolver size is too small (%d)",
            name, tOpCount);
      }
      return kTfLiteError;
    }
    TfLiteRegistration* new_registration = &registrations_[registrations_len_];
    registrations_len_ += 1;

    *new_registration = *registration;
    new_registration->builtin_code = BuiltinOperator_CUSTOM;
    new_registration->custom_name = name;
    new_registration->version = version;

    return kTfLiteOk;
  }

  TfLiteStatus AddCustom(const char* name, TfLiteRegistration* registration,
                         int min_version, int max_version) {
    for (int version = min_version; version <= max_version; ++version) {
      TfLiteStatus add_status = AddCustom(name, registration, version);
      if (add_status != kTfLiteOk) {
        return add_status;
      }
    }
    return kTfLiteOk;
  }

  unsigned int GetRegistrationLength() { return registrations_len_; }

 private:
  TfLiteRegistration registrations_[tOpCount];
  unsigned int registrations_len_ = 0;
  ErrorReporter* error_reporter_;

  TF_LITE_REMOVE_VIRTUAL_DELETE
};

// TODO(b/147854028): Consider switching all uses of MicroMutableOpResolver to
// MicroOpResolver.
class MicroMutableOpResolver
    : public MicroOpResolver<TFLITE_REGISTRATIONS_MAX> {
 private:
  TF_LITE_REMOVE_VIRTUAL_DELETE
};

};  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_MICRO_MUTABLE_OP_RESOLVER_H_
