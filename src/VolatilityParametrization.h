/*
 * VolatilityParametrization.h
 *
 *  Created on: Nov 17, 2009
 *      Author: dupontr
 */

#ifndef VOLATILITYPARAMETRIZATION_H_
#define VOLATILITYPARAMETRIZATION_H_

/// \brief An abstract class to represent a parametrization
/// as a function of (forward) moneyness, the moneyness being
/// defined in terms of ln(K / F_T).
class VolatilityParametrization
{
public:
	virtual double evaluateVolatility(double moneyness) const = 0;
	virtual double evaluateVolatilityDerivative(double moneyness) const = 0;
};

/// \brief A simple, quadratic parametrization of volatility
class VolatilityQuadraticForm : public VolatilityParametrization
{
public:
	/// Constructor
	VolatilityQuadraticForm(
		double atm_fwd_volatility,
		double skew,
		double smile)
	:mAtmFwdVolatility(atm_fwd_volatility)
	,mSkew(skew)
	,mSmile(smile)
	{}

	double evaluateVolatility(double moneyness) const
	{
		return mAtmFwdVolatility + moneyness * (mSkew + moneyness * mSmile);
	}

	double evaluateVolatilityDerivative(double moneyness) const
	{
		return mSkew + 0.5 * mSmile * moneyness;
	}

private:
	double mAtmFwdVolatility;
	double mSkew;
	double mSmile;
};

#endif /* VOLATILITYPARAMETRIZATION_H_ */
