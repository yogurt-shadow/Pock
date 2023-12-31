/*++
Copyright (c) 2006 Microsoft Corporation

Module Name:

    region.cpp

Abstract:

    Region/Arena memory manager

Author:

    Leonardo de Moura (leonardo) 2006-09-18.

Revision History:

--*/
#include "region.h"

#ifdef Z3DEBUG

void region::display_mem_stats(std::ostream & out) const {
    out << "num. objects:      " << m_chunks.size() << "\n";
}

void * region::allocate(size_t size) {
    char * r = alloc_svect(char, size);
    m_chunks.push_back(r);
    return r;
}

void region::reset() {
    for (auto* c : m_chunks)
        dealloc_svect(c);
    m_chunks.reset();
    m_scopes.reset();
}

void region::pop_scope() {
    unsigned old_size = m_scopes.back();
    m_scopes.pop_back();
    ptr_vector<char>::iterator it  = m_chunks.begin() + old_size;
    ptr_vector<char>::iterator end = m_chunks.end();
    for (; it != end; ++it) 
        dealloc_svect(*it);    
    m_chunks.shrink(old_size);
}


#else

#include "tptr.h"
#include "debug.h"
#include "memory_manager.h"
#include "page.h"

inline void region::allocate_page() {
    m_curr_page     = allocate_default_page(m_curr_page, m_free_pages);
    m_curr_ptr      = m_curr_page;
    m_curr_end_ptr  = end_of_default_page(m_curr_page);
}

region::region() {
    m_curr_page    = nullptr;
    m_curr_ptr     = nullptr;
    m_curr_end_ptr = nullptr;
    m_free_pages   = nullptr;
    m_mark         = nullptr;
    allocate_page();
}

region::~region() {
    del_pages(m_curr_page);
    del_pages(m_free_pages);
}

void * region::allocate(size_t size) {
    char * new_curr_ptr = m_curr_ptr + size;
    if (new_curr_ptr < m_curr_end_ptr) {
        char * result = m_curr_ptr;
        m_curr_ptr = ALIGN(char *, new_curr_ptr);
        return result;
    }
    else if (size < DEFAULT_PAGE_SIZE) {
        allocate_page(); 
        char * result = m_curr_ptr;
        m_curr_ptr += size;
        m_curr_ptr = ALIGN(char *, m_curr_ptr);
        return result;
    }
    else {
        // big page
        m_curr_page = ::allocate_page(m_curr_page, size);
        char * result = m_curr_page;
        allocate_page();
        return result;
    }
}

inline void region::recycle_curr_page() {
    char * prev = prev_page(m_curr_page);
    recycle_page(m_curr_page, m_free_pages);
    m_curr_page = prev;
}

void region::reset() {
    while (m_curr_page != nullptr) {
        recycle_curr_page();
    }
    SASSERT(m_curr_page == 0);
    m_curr_ptr     = nullptr;
    m_curr_end_ptr = nullptr;
    m_mark         = nullptr;
    allocate_page();
}

void region::push_scope() {
    char * curr_page = m_curr_page;
    char * curr_ptr  = m_curr_ptr;
    m_mark = new (*this) mark(curr_page, curr_ptr, m_mark);
}

void region::pop_scope() {
    SASSERT(m_mark);
    char * old_curr_page = m_mark->m_curr_page;
    SASSERT(is_default_page(old_curr_page));
    m_curr_ptr           = m_mark->m_curr_ptr;
    m_mark               = m_mark->m_prev_mark;
    while (m_curr_page != old_curr_page) {
        recycle_curr_page();
    }
    SASSERT(is_default_page(m_curr_page));
    m_curr_end_ptr       = end_of_default_page(m_curr_page);
}

void region::display_mem_stats(std::ostream & out) const {
    unsigned n = 0;
    char * page = m_curr_page;
    while (page != nullptr) {
        n++;
        page = prev_page(page);
    }
    out << "num. pages:      " << n << "\n";
}

#endif

