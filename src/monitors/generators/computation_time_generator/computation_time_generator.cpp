//
// Created by Evald Nexhipi on 28.10.21.
//
#include "computation_time_generator.h"
#include <chrono>

std::shared_ptr<Expression> ComputationTimeGenerator::generateAtomOrPredicate() {
    double coin;
    coin = rreal(0, m_sumWeightsAtomic + m_sumWeightsPredicate);
    double w = 0.0;
    for(auto & iter : m_atoms)
    {
        w += iter.m_weight;
        if(w >= coin)
            return iter.m_atomic;
    }
    for(auto & iter : m_predicates)
    {
        w += iter.m_weight;
        if(w >= coin)
            return iter.m_predicate;
    }
    if(m_predicates.size() > 0)
	return m_predicates.back().m_predicate;
    if(m_atoms.size() > 0)
	return m_atoms.back().m_atomic;    
    return NULL;
    
}

void ComputationTimeGenerator::generateAtomsAndPredicates(int nrPreds, std::vector < std::shared_ptr<Expression> > & preds)
{
    for(int i = 0; i  < nrPreds; ++i)
	preds.push_back(generateAtomOrPredicate());
}

double ComputationTimeGenerator::createPredicatesOnDelays (std::vector<int> info,  std::vector< std::vector<std::shared_ptr<Expression>>*> &allPreds)
{
    for(int i = 0; i < info.size(); i += 3)
    {
	auto timeDelay = info[i];
	auto minNrPreds = info[i + 1];
	auto maxNrPreds = info[i + 2];
	auto nrPreds = rint(minNrPreds, maxNrPreds);

	auto preds = new std::vector<std::shared_ptr<Expression>>();
	generateAtomsAndPredicates(nrPreds, *preds);
	allPreds.push_back(preds);
	
    }

    return 0.0;
    
}

