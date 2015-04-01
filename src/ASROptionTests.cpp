//
//  ASROptionTests.cpp
//  GenPDE
//
//  Created by Regis Dupont on 3/15/15.
//  Copyright (c) 2015 Regis Dupont. All rights reserved.
//

#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Framework.h"
#include "TestSuite.h"

#include "Date.h"
#include "AuxiliaryVariable.h"
#include "GenPDEParser.h"
#include "PDEPricingModelBlackScholes.h"
#include "PDETradePricer.h"

using std::vector;
using std::cerr;
using std::endl;
using namespace GenPDE;

REGISTER_TEST(ASROption1)
{
    // Payout:
    // S_T / Max( A_T, 90 )
    
    //boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();
    
    std::string file("/Users/regisdupont/Documents/Code/GenPDE/GenPDE/Resources/ASROption.xml");
    boost::shared_ptr<TradeRepresentation> tradeRepresentation(GenPDEParser::parseTradeRepresentation(file, true));
    
    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    //std::cout << "Finished parsing trade representation (" << (mst1 - mst0).total_microseconds() << ")" << std::endl;
    
    GenPDE::Date d0(GenPDE::dateFromString("20020101"));
	const double spot(100.0);
	const double rate(0.05);
	const double volatility(0.25);
	const size_t nbSpaceNodes(1000);
    const size_t nbPlanes(1000);
	const double stdDevMultiple(5.0);
    const size_t nbRannacher(4);
    
    // For now, we hardcode the discretization of the AVs:
    boost::shared_ptr<AVContext> avContext = tradeRepresentation->getAVContext();
    boost::shared_ptr<AuxiliaryVariable> av0 = avContext->getAuxiliaryVariable(0);
    std::vector<double> avValues0(1, 100.0);
    GenPDE::Date        prevDate   = av0->getDate();
    double              totalTenor = 0;
    av0->setDiscretizationValues(avValues0);
    for(size_t i=1; i<126; ++i)
    {
        boost::shared_ptr<AuxiliaryVariable> av = avContext->getAuxiliaryVariable(i);
        GenPDE::Date thisDate = av->getDate();
        totalTenor           += GenPDE::dateDifferenceInDays(thisDate, prevDate) / 365.0;
        double stdDev         = volatility * Double::sqrt(totalTenor);
        double width          = 2 * stdDevMultiple * stdDev;
        double lowerBound     = Double::log(i * spot) - 0.5 * width;
        double ds             = width / ((double) (nbPlanes-1));
        std::vector<double> avValues(nbPlanes);
        for(size_t j=0; j<nbPlanes; ++j)
            avValues[j] = Double::exp(lowerBound + j * ds);
        av->setDiscretizationValues(avValues);
        prevDate              = thisDate;
    }
    
    boost::shared_ptr<PDEPricingModelInterface> model(new PDEPricingModelBlackScholes(
        d0,
        spot,
        rate,
        volatility,
        3,
        nbRannacher,
        0.3,
        nbSpaceNodes,
        stdDevMultiple
    ));
    
    boost::shared_ptr<PDETradePricer> pricer(new PDETradePricer(model, tradeRepresentation));
    double price(pricer->price());
    boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
    std::cout << "Finished pricing (" << (mst2 - mst1).total_microseconds() << ")" << std::endl;
    std::cout << "Price: " << price << std::endl;
    //double cf = BlackScholes::callPV(spot, rate, volatility / Double::sqrt(3), 1.0, 100);
}