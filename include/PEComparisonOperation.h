#ifndef PE_COMPARISON_OPERATION_H
#define PE_COMPARISON_OPERATION_H

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "PayoutExpression.h"
#include "PEOperators.h"

class PEComparisonOperation : public PayoutExpression
{
    typedef boost::shared_ptr<PayoutExpression> PEPtr;
    
public:
    PEComparisonOperation(
        GenPDE::ComparisonOperator op,
        PEPtr                      pe1,
        PEPtr                      pe2
    )
    :mOperator(op)
    ,mPE1(pe1)
    ,mPE2(pe2)
    {}
    
    virtual CEVConstPtr evalCE(
        const MOContext& mo_context,
        const AVContext& av_context
    ) const;
    
    virtual CEVConstPtr evalCE(
        const PDEModelPtr& model
    ) const;

    virtual CEVConstPtr evalFromFixings(
        const GenPDE::Date&       date,
        const MOFixingsIfc&       mo_fixings,
        const AuxiliaryVariables& av_defs,
        AVContext&                av_context // updated by the call
    ) const;
    
protected:
    virtual void print(std::ostream& stream) const;
    
protected:
    GenPDE::ComparisonOperator mOperator;
    PEPtr                      mPE1;
    PEPtr                      mPE2;
};

#endif // PE_COMPARISON_OPERATION_H
