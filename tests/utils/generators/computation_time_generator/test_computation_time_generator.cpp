//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "test_computation_time_generator.h"
#include "monitors/utils/generators/computation_time_generator/computation_time_generator.h"

void print_preds (std::vector<std::vector<std::shared_ptr<Expression>> *> &allPreds){
        for (auto &preds : allPreds){
            std::cout<<"Preds Generated: ";
            for (auto &pred: *preds){
                std::cout<<pred->getSymbol()<<" ";
            }
            std::cout<<std::endl;
        }
}

TEST(TestComputationTimeGenerator, tctg) {
    ComputationTimeGenerator ctg;
    ComputationTimeGenerator::rseed();
    std::vector<std::vector<std::shared_ptr<Expression>> *> allPreds;

    /* Atoms and Predicates of Delay 1 */
    auto a11 = std::make_shared<Atomic>("a11");
    auto a12 = std::make_shared<Atomic>("a12");
    auto a13 = std::make_shared<Atomic>("a13");
    auto p11 = std::make_shared<Predicate>("p11", "p11", std::vector<std::string>{"params1", "params2"});
    auto p12 = std::make_shared<Predicate>("p12", "p12", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {a11, a12, a13};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {p11, p12};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctg.addAtomsDistribution(atoms_1, atom_weights_1);
    ctg.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Atoms and Predicates of Delay 2 */
    auto a21 = std::make_shared<Atomic>("a21");
    auto a22 = std::make_shared<Atomic>("a22");
    auto p21 = std::make_shared<Predicate>("p21", "p21", std::vector<std::string>{"params1", "params2"});
    auto p22 = std::make_shared<Predicate>("p22", "p22", std::vector<std::string>{"params1", "params2", "params3"});
    auto p23 = std::make_shared<Predicate>("p23", "p23", std::vector<std::string>{"params1", "params2", "params3"});
    auto p24 = std::make_shared<Predicate>("p24", "p24", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {a21, a22};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {p21, p22, p23, p24};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctg.addAtomsDistribution(atoms_2, atom_weights_2);
    ctg.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Atoms and Predicates of Delay 3 */
    auto a31 = std::make_shared<Atomic>("a31");
    auto a32 = std::make_shared<Atomic>("a32");
    auto a33 = std::make_shared<Atomic>("a33");
    auto a34 = std::make_shared<Atomic>("a34");
    auto p31 = std::make_shared<Predicate>("p31", "p31", std::vector<std::string>{"params1", "params2"});
    auto p32 = std::make_shared<Predicate>("p32", "p32", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {a31, a32, a33, a34};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {p31, p32};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctg.addAtomsDistribution(atoms_3, atom_weights_3);
    ctg.addPredicateDistribution(predicates_3, predicate_weights_3);

    /* Atoms and Predicates of Delay 4 */
    auto a41 = std::make_shared<Atomic>("a41");
    auto a42 = std::make_shared<Atomic>("a42");
    auto p41 = std::make_shared<Predicate>("p41", "p41", std::vector<std::string>{"params1", "params2"});
    auto p42 = std::make_shared<Predicate>("p42", "p41", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 4 */
    std::vector<std::shared_ptr<Atomic>> atoms_4 = {a41, a42};
    std::vector<std::shared_ptr<Predicate>> predicates_4 = {p41, p42};
    std::vector<double> atom_weights_4 = {1.0, 1.0};
    std::vector<double> predicate_weights_4 = {2.0, 1.0};
    ctg.addAtomsDistribution(atoms_4, atom_weights_4);
    ctg.addPredicateDistribution(predicates_4, predicate_weights_4);

    ctg.createPredicatesOnDelays({500, 1, 3, 1000, 2, 6, 750, 1, 5, 2000, 1, 4}, allPreds);

    //print_preds(allPreds);

    EXPECT_TRUE(true);
}

TEST(TestComputationTimeGeneratorLongerDelays, tctgld) {
    ComputationTimeGenerator ctg;
    ComputationTimeGenerator::rseed();
    std::vector<std::vector<std::shared_ptr<Expression>> *> allPreds;

    /* Atoms and Predicates of Delay 1 */
    auto a11 = std::make_shared<Atomic>("a11");
    auto a12 = std::make_shared<Atomic>("a12");
    auto a13 = std::make_shared<Atomic>("a13");
    auto p11 = std::make_shared<Predicate>("p11", "p11", std::vector<std::string>{"params1", "params2"});
    auto p12 = std::make_shared<Predicate>("p12", "p12", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {a11, a12, a13};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {p11, p12};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctg.addAtomsDistribution(atoms_1, atom_weights_1);
    ctg.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Atoms and Predicates of Delay 2 */
    auto a21 = std::make_shared<Atomic>("a21");
    auto a22 = std::make_shared<Atomic>("a22");
    auto p21 = std::make_shared<Predicate>("p21", "p21", std::vector<std::string>{"params1", "params2"});
    auto p22 = std::make_shared<Predicate>("p22", "p22", std::vector<std::string>{"params1", "params2", "params3"});
    auto p23 = std::make_shared<Predicate>("p23", "p23", std::vector<std::string>{"params1", "params2", "params3"});
    auto p24 = std::make_shared<Predicate>("p24", "p24", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {a21, a22};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {p21, p22, p23, p24};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctg.addAtomsDistribution(atoms_2, atom_weights_2);
    ctg.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Atoms and Predicates of Delay 3 */
    auto a31 = std::make_shared<Atomic>("a31");
    auto a32 = std::make_shared<Atomic>("a32");
    auto a33 = std::make_shared<Atomic>("a33");
    auto a34 = std::make_shared<Atomic>("a34");
    auto p31 = std::make_shared<Predicate>("p31", "p31", std::vector<std::string>{"params1", "params2"});
    auto p32 = std::make_shared<Predicate>("p32", "p32", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {a31, a32, a33, a34};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {p31, p32};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctg.addAtomsDistribution(atoms_3, atom_weights_3);
    ctg.addPredicateDistribution(predicates_3, predicate_weights_3);

    /* Atoms and Predicates of Delay 4 */
    auto a41 = std::make_shared<Atomic>("a41");
    auto a42 = std::make_shared<Atomic>("a42");
    auto p41 = std::make_shared<Predicate>("p41", "p41", std::vector<std::string>{"params1", "params2"});
    auto p42 = std::make_shared<Predicate>("p42", "p42", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 4 */
    std::vector<std::shared_ptr<Atomic>> atoms_4 = {a41, a42};
    std::vector<std::shared_ptr<Predicate>> predicates_4 = {p41, p42};
    std::vector<double> atom_weights_4 = {1.0, 1.0};
    std::vector<double> predicate_weights_4 = {2.0, 1.0};
    ctg.addAtomsDistribution(atoms_4, atom_weights_4);
    ctg.addPredicateDistribution(predicates_4, predicate_weights_4);

    ctg.createPredicatesOnDelays({5000, 1, 3, 1000, 2, 6, 7500, 1, 5, 3000, 1, 4}, allPreds);
    EXPECT_TRUE(true);
}

TEST(TestComputationTimeGeneratorHighNumberOfPredicates, tctghnop) {
    ComputationTimeGenerator ctg;
    ComputationTimeGenerator::rseed();
    std::vector<std::vector<std::shared_ptr<Expression>> *> allPreds;

    /* Atoms and Predicates of Delay 1 */
    auto a11 = std::make_shared<Atomic>("a11");
    auto a12 = std::make_shared<Atomic>("a12");
    auto a13 = std::make_shared<Atomic>("a13");
    auto p11 = std::make_shared<Predicate>("p11", "p11", std::vector<std::string>{"params1", "params2"});
    auto p12 = std::make_shared<Predicate>("p12", "p12", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {a11, a12, a13};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {p11, p12};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctg.addAtomsDistribution(atoms_1, atom_weights_1);
    ctg.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Atoms and Predicates of Delay 2 */
    auto a21 = std::make_shared<Atomic>("a21");
    auto a22 = std::make_shared<Atomic>("a22");
    auto p21 = std::make_shared<Predicate>("p21", "p21", std::vector<std::string>{"params1", "params2"});
    auto p22 = std::make_shared<Predicate>("p22", "p22", std::vector<std::string>{"params1", "params2", "params3"});
    auto p23 = std::make_shared<Predicate>("p23", "p23", std::vector<std::string>{"params1", "params2", "params3"});
    auto p24 = std::make_shared<Predicate>("p24", "p24", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {a21, a22};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {p21, p22, p23, p24};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctg.addAtomsDistribution(atoms_2, atom_weights_2);
    ctg.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Atoms and Predicates of Delay 3 */
    auto a31 = std::make_shared<Atomic>("a31");
    auto a32 = std::make_shared<Atomic>("a32");
    auto a33 = std::make_shared<Atomic>("a33");
    auto a34 = std::make_shared<Atomic>("a34");
    auto p31 = std::make_shared<Predicate>("p31", "p31", std::vector<std::string>{"params1", "params2"});
    auto p32 = std::make_shared<Predicate>("p32", "p32", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {a31, a32, a33, a34};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {p31, p32};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctg.addAtomsDistribution(atoms_3, atom_weights_3);
    ctg.addPredicateDistribution(predicates_3, predicate_weights_3);

    /* Atoms and Predicates of Delay 4 */
    auto a41 = std::make_shared<Atomic>("a41");
    auto a42 = std::make_shared<Atomic>("a42");
    auto p41 = std::make_shared<Predicate>("p41", "p41", std::vector<std::string>{"params1", "params2"});
    auto p42 = std::make_shared<Predicate>("p42", "p42", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 4 */
    std::vector<std::shared_ptr<Atomic>> atoms_4 = {a41, a42};
    std::vector<std::shared_ptr<Predicate>> predicates_4 = {p41, p42};
    std::vector<double> atom_weights_4 = {1.0, 1.0};
    std::vector<double> predicate_weights_4 = {2.0, 1.0};
    ctg.addAtomsDistribution(atoms_4, atom_weights_4);
    ctg.addPredicateDistribution(predicates_4, predicate_weights_4);

    ctg.createPredicatesOnDelays({1000, 10, 50, 2000, 2, 20, 3000, 3, 50, 4000, 1, 40}, allPreds);
    EXPECT_TRUE(true);
}

TEST(TestComputationTimeGeneratorNoPredicates, tctgnp) {
    ComputationTimeGenerator ctg;
    ComputationTimeGenerator::rseed();
    std::vector<std::vector<std::shared_ptr<Expression>> *> allPreds;

    /* Atoms and Predicates of Delay 1 */
    auto a11 = std::make_shared<Atomic>("a11");
    auto a12 = std::make_shared<Atomic>("a12");
    auto a13 = std::make_shared<Atomic>("a13");
    auto p11 = std::make_shared<Predicate>("p11", "p11", std::vector<std::string>{"params1", "params2"});
    auto p12 = std::make_shared<Predicate>("p12", "p12", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {a11, a12, a13};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {p11, p12};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctg.addAtomsDistribution(atoms_1, atom_weights_1);
    ctg.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Atoms and Predicates of Delay 2 */
    auto a21 = std::make_shared<Atomic>("a21");
    auto a22 = std::make_shared<Atomic>("a22");
    auto p21 = std::make_shared<Predicate>("p21", "p21", std::vector<std::string>{"params1", "params2"});
    auto p22 = std::make_shared<Predicate>("p22", "p22", std::vector<std::string>{"params1", "params2", "params3"});
    auto p23 = std::make_shared<Predicate>("p23", "p23", std::vector<std::string>{"params1", "params2", "params3"});
    auto p24 = std::make_shared<Predicate>("p24", "p24", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {a21, a22};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {p21, p22, p23, p24};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctg.addAtomsDistribution(atoms_2, atom_weights_2);
    ctg.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Atoms and Predicates of Delay 3 */
    auto a31 = std::make_shared<Atomic>("a31");
    auto a32 = std::make_shared<Atomic>("a32");
    auto a33 = std::make_shared<Atomic>("a33");
    auto a34 = std::make_shared<Atomic>("a34");
    auto p31 = std::make_shared<Predicate>("p31", "p31", std::vector<std::string>{"params1", "params2"});
    auto p32 = std::make_shared<Predicate>("p32", "p32", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {a31, a32, a33, a34};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {p31, p32};
    std::vector<double> atom_weights_3 = {2.0, 4.0, 3.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 4.0};
    ctg.addAtomsDistribution(atoms_3, atom_weights_3);
    ctg.addPredicateDistribution(predicates_3, predicate_weights_3);

    /* Atoms and Predicates of Delay 4 */
    auto a41 = std::make_shared<Atomic>("a41");
    auto a42 = std::make_shared<Atomic>("a42");
    auto p41 = std::make_shared<Predicate>("p41", "p41", std::vector<std::string>{"params1", "params2"});
    auto p42 = std::make_shared<Predicate>("p42", "p42", std::vector<std::string>{"params1", "params2", "params3"});
    /* Distribution 4 */
    std::vector<std::shared_ptr<Atomic>> atoms_4 = {a41, a42};
    std::vector<std::shared_ptr<Predicate>> predicates_4 = {p41, p42};
    std::vector<double> atom_weights_4 = {1.0, 1.0};
    std::vector<double> predicate_weights_4 = {2.0, 1.0};
    ctg.addAtomsDistribution(atoms_4, atom_weights_4);
    ctg.addPredicateDistribution(predicates_4, predicate_weights_4);

    ctg.createPredicatesOnDelays({1000, 0, 0, 2000, 2, 20, 1000, 0, 0, 1000, 0, 0}, allPreds);
    EXPECT_TRUE(true);
}

TEST(TestComputationTimeGeneratorNoExpression, tctgne) {
    ComputationTimeGenerator ctg;
    ComputationTimeGenerator::rseed();
    std::vector<std::vector<std::shared_ptr<Expression>> *> allPreds;

    ctg.createPredicatesOnDelays({1000, 0, 0, 2000, 2, 20, 1000, 0, 0, 1000, 0, 0}, allPreds);
    EXPECT_TRUE(true);
}