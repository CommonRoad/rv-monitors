//
// Created by Evald Nexhipi on 28.10.21.
//
#include "computation_time_generator.h"
#include <chrono>

std::shared_ptr<Expression> ComputationTimeGenerator::generateAtomOrPredicate(int delay_index) {
    double coin;
    coin = rreal(0, m_sumWeightsAtomic[delay_index] + m_sumWeightsPredicate[delay_index]);
    double w = 0.0;
    for(auto & iter : m_atoms[delay_index])
    {
        w += iter.m_weight;
        if(w >= coin)
            return iter.m_atomic;
    }
    for(auto & iter : m_predicates[delay_index])
    {
        w += iter.m_weight;
        if(w >= coin)
            return iter.m_predicate;
    }
    if(m_predicates[delay_index].size() > 0)
        return m_predicates[delay_index].back().m_predicate;
    if(m_atoms[delay_index].size() > 0)
        return m_atoms[delay_index].back().m_atomic;
    return NULL;
}

void ComputationTimeGenerator::generateAtomsAndPredicates(int index_of_delay, int nrPreds, std::vector < std::shared_ptr<Expression> > & preds)
{
//    std::vector<std::shared_ptr<Expression>> predicates_ith_delay;
//    int max_iterations = (m_atoms.size() >= m_predicates.size()) ? m_atoms.size() : m_predicates.size();
//    for (int j=0; j< max_iterations; ++j) {
        for (int i = 0; i < nrPreds; ++i)
            preds.push_back(generateAtomOrPredicate(index_of_delay));
//    }
}

void ComputationTimeGenerator::createPredicatesOnDelays (std::vector<int> info,  std::vector< std::vector<std::shared_ptr<Expression>>*> &allPreds)
{
    int next_delay = 0;
    for(int i = 0; i < info.size(); i += 3)
    {
	auto timeDelay = info[i];
	auto minNrPreds = info[i + 1];
	auto maxNrPreds = info[i + 2];
	auto nrPreds = rint(minNrPreds, maxNrPreds);

	auto preds = new std::vector<std::shared_ptr<Expression>>();
        std::cout<<"Delay "<<next_delay+1<<" started"<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
        generateAtomsAndPredicates(next_delay, nrPreds, *preds);
        std::cout<<"Predicates Generated: ";
        for (auto & p : *preds)
            std::cout<<p->to_string()<<" ";
        std::cout<<std::endl;
        next_delay++;
	allPreds.push_back(preds);
    }
}

