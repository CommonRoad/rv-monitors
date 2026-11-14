#pragma once

#include "monitors/collections/domain.h"
#include <gtest/gtest.h>

class TestDomain : public testing::Test {
  protected:
    DomainStorage dom1;
    DomainStorage dom2;
    DomainStorage dom3;

  private:
    void SetUp() override;
};
