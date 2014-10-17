#ifndef EULERI_H
#define EULERI_H

#include "Euler.h"

template <class DOMAIN_TYPE, class STATE_TYPE>
Euler<DOMAIN_TYPE, STATE_TYPE>::Euler()
    :
      Solver("Explicit Euler")
{

}

template <class DOMAIN_TYPE, class STATE_TYPE>
void Euler<DOMAIN_TYPE, STATE_TYPE>::initialize(Input &input)
{

    Solver::initialize(input);

    Solver::initNumOfSteps(1, domain_.size());

}

template <class DOMAIN_TYPE, class STATE_TYPE>
void Euler<DOMAIN_TYPE, STATE_TYPE>::solve()
{



}


#endif