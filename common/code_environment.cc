// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2011 New Dream Network
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */

#include "common/code_environment.h"

#include <iostream>

#include "include/acconfig.h"

#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#endif

#include <string.h>

code_environment_t g_code_env = CODE_ENVIRONMENT_UTILITY;

extern "C" const char *code_environment_to_str(enum code_environment_t e)
{
  switch (e) {
    case CODE_ENVIRONMENT_UTILITY:
      return "CODE_ENVIRONMENT_UTILITY";
    case CODE_ENVIRONMENT_DAEMON:
      return "CODE_ENVIRONMENT_DAEMON";
    case CODE_ENVIRONMENT_LIBRARY:
      return "CODE_ENVIRONMENT_LIBRARY";
    default:
      return NULL;
  }
}

std::ostream &operator<<(std::ostream &oss, const enum code_environment_t e)
{
  oss << code_environment_to_str(e);
  return oss;
}

#if defined(HAVE_SYS_PRCTL_H) && defined(PR_GET_NAME) /* Since 2.6.11 */

int get_process_name(char *buf, int len)
{
  if (len <= 16) {
    /* The man page discourages using this prctl with a buffer shorter
     * than 16 bytes. With a 16-byte buffer, it might not be
     * null-terminated. */
    return -ENAMETOOLONG;
  }
  // FIPS zeroization audit 20191115: this memset is not security related.
  memset(buf, 0, len);
  return prctl(PR_GET_NAME, buf);
}

#elif defined(HAVE_GETPROGNAME)

int get_process_name(char *buf, int len)
{
  if (len <= 0) {
    return -EINVAL;
  }

  const char *progname = getprogname();
  if (progname == nullptr || *progname == '\0') {
    return -ENOSYS;
  }

  strncpy(buf, progname, len - 1);
  buf[len - 1] = '\0';
  return 0;
}

#else

int get_process_name(char *buf, int len)
{
  return -ENOSYS;
}

#endif

std::string get_process_name_cpp()
{
  char buf[32];
  if (get_process_name(buf, sizeof(buf))) {
    return "(unknown)";
  }
  return std::string(buf);
}
