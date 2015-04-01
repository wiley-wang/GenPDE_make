//
//  PayoutExpression.h
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef PAYOUTEXPRESSION_H_
#define PAYOUTEXPRESSION_H_

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "CEValues.h"
#include "MOContext.h"
#include "AVContext.h"
#include "PDEPricingModelInterface.h"

class PayoutExpression
{
protected:
    typedef boost::shared_ptr<const CEValues> CEVConstPtr;
    typedef boost::shared_ptr<const PDEPricingModelInterface> PDEModelPtr;
    
public:
    virtual ~PayoutExpression(){}
    
    virtual CEVConstPtr evalCE(
        const MOContext& mo_context,
        const AVContext& av_context
    ) const = 0;
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const = 0;
    
    friend std::ostream& operator<<(std::ostream& stream, const PayoutExpression& pe);
    
protected:
    virtual void print(std::ostream& stream) const = 0;
};

#endif // PAYOUTEXPRESSION_H_