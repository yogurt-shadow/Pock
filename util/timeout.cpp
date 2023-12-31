/*++
Copyright (c) 2006 Microsoft Corporation

Module Name:

    timeout.cpp

Abstract:

    Timeout support

Author:

    Leonardo de Moura (leonardo) 2006-10-02.

Revision History:

    Christoph (cwinter) 2012-02-14: Switch to scoped_timer for timeout support on all platforms.

--*/
#include<iostream>
#include "util.h"
#include "timeout.h"
#include "error_codes.h"

#include "event_handler.h"
#include "scoped_timer.h"

static scoped_timer * g_timeout = nullptr;
static void (* g_on_timeout)() = nullptr;

namespace {
class g_timeout_eh : public event_handler {
public:
    void operator()(event_handler_caller_t caller_id) override {
        m_caller_id = caller_id;
        std::cout << "timeout\n";
        std::cout.flush();
        if (g_on_timeout)
            g_on_timeout();
        throw z3_error(ERR_TIMEOUT);
    }
};
}

static g_timeout_eh eh;

void set_timeout(long ms) {
    SASSERT(!g_timeout);
    g_timeout = new scoped_timer(ms, &eh);
}

void disable_timeout() {
    delete g_timeout;
}

void register_on_timeout_proc(void (*proc)()) {
    g_on_timeout = proc;
}
