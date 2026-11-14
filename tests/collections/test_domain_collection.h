#pragma once

#include "monitors/collections/domain_collection.h"
#include <gtest/gtest.h>

class TestDomainCollection : public testing::Test {
  protected:
    DomainStorage dom1;
    DomainStorage dom2;
    DomainCollection domC1;
    DomainCollection domC2;

  private:
    void SetUp() override;
};
