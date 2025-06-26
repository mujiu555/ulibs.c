#ifndef ULIB__LOG_H__
#define ULIB__LOG_H__

#include <stdio.h>

extern FILE  *log_fout;
extern size_t log_level_indent;

#define LOGD(msg, level)                                                       \
  fprintf(                                                                     \
    log_fout,                                                                  \
    "[%-*s] %s: l%d; %s\n",                                                    \
    log_level_indent,                                                          \
    level,                                                                     \
    __FILE__,                                                                  \
    __LINE__,                                                                  \
    msg                                                                        \
  )

#define LOGF(fmt, level, ...)                                                  \
  fprintf(                                                                     \
    log_fout,                                                                  \
    ("[%-*s] %s: l%d; " fmt "\n"),                                             \
    log_level_indent,                                                          \
    level,                                                                     \
    __FILE__,                                                                  \
    __LINE__,                                                                  \
    ##__VA_ARGS__                                                              \
  )

// TODO: log instructions

#endif // !ULIB__LOG_H__
