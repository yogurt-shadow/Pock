/*++
Copyright (c) 2011 Microsoft Corporation

Module Name:

    prime_generator.h

Abstract:

    Prime generator

Author:

    Leonardo (leonardo) 2011-12-23

Notes:

--*/
#pragma once

#include "vector.h"
#include "z3_exception.h"
#include "util.h"

class prime_generator_exception : public default_exception {
public:
    prime_generator_exception(char const * msg):default_exception(msg) {}
};

/**
   \brief Prime generator
*/
class prime_generator {
    svector<uint64_t> m_primes;
    void process_next_k_numbers(uint64_t k);
public:
    uint64_t operator()(unsigned idx);
    void initialize();
    void finalize();
};

class prime_iterator {
    unsigned          m_idx;
    prime_generator * m_generator;
    bool              m_global;
public:
    prime_iterator(prime_generator * g = nullptr);
    uint64_t next();
    static void initialize();
    static void finalize();
    /*
      ADD_INITIALIZER('prime_iterator::initialize();')
      ADD_FINALIZER('prime_iterator::finalize();')
    */
};

