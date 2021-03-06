// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_BACKTRACE_H
#define CEPH_BACKTRACE_H

#include "include/acconfig.h"
#include <iosfwd>
#ifdef HAVE_EXECINFO_H
#include <execinfo.h>
#endif
#include <stdlib.h>

namespace ceph {

class Formatter;

struct BackTrace {
  const static int max = 100;

  int skip;
  void *array[max]{};
  size_t size;
  char **strings;

  explicit BackTrace(int s) : skip(s) {
#ifdef HAVE_EXECINFO_H
    size = backtrace(array, max);
    strings = backtrace_symbols(array, size);
#else
    skip = 0;
    size = 0;
    strings = nullptr;
#endif
  }
  ~BackTrace() {
    free(strings);
  }

  BackTrace(const BackTrace& other);
  const BackTrace& operator=(const BackTrace& other);

  void print(std::ostream& out) const;
  void dump(Formatter *f) const;
};

inline std::ostream& operator<<(std::ostream& out, const BackTrace& bt) {
  bt.print(out);
  return out;
}

}

#endif
