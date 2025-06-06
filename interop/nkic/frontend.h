/*
Copyright (c) 2025 Amazon.com, Inc. or its affiliates. All Rights Reserved.
Released under Apache 2.0 license as described in the file LICENSE.
*/
#pragma once
#include "stdc.h"
#include "region.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>
static_assert(
    PY_MAJOR_VERSION == 3 &&
    PY_MINOR_VERSION >= 9 &&
    PY_MINOR_VERSION <= 12,
    "Unsupported Python Version");

// Front-end version (place holder)
#define KLR_VERSION 1

// The place where we live
//#define MODULE_ROOT "neuronxcc.nki"
#define MODULE_ROOT ""

// The front-end is accessed through the class Kernel; one instance
// per kernel. Each instance has a `struct kernel` on the C side.

struct kernel {
  PyObject_HEAD
  PyObject *f;   // Kernel function
  bool specialized;
  struct region *region;
  struct Python_Kernel *python_kernel;
  struct NKI_Kernel *nki_kernel;
};

// peg_parser.c
struct _mod* parse_string(const char *str, PyObject* filename);
void free_python_ast(struct _mod *m);

// gather.c
bool gather(struct kernel *k);
