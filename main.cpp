#include "polynomial/polynomial.h"
#include <iostream>

typedef polynomial::var            var;
typedef polynomial::var_vector     var_vector;
typedef polynomial::manager        pmanager;
typedef polynomial::polynomial     poly;
typedef polynomial::monomial       monomial;
typedef polynomial::numeral        numeral;

int main() {
    reslimit r;
    unsynch_mpq_manager    m_qm;
    // mpq rr(12);
    // m_qm.div(rr, mpq(14), rr);
    // m_qm.display(std:cout, rr);
    // std::cout << std::endl;

    // pmanager m_pm(r, m_qm);
    // poly * p = m_pm.mk_const(rr);
    // m_pm.div(p, m_pm.mk_const(mpq(14)), p);
    // m_pm.display(std::cout, p);
    // std::cout << std::endl;

    // poly * q = m_pm.mk_const(rational(-3));
    // p = m_pm.add(p, q);
    // m_pm.display(std::cout, p);

    numeral rr(0.3);
    m_qm.display(std::cout, rr);
    pmanager m_pm(r, m_qm);
    poly* p = m_pm.mk_const(rr);
    m_pm.display(std::cout, p);

    return 0;
}