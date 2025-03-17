#pragma once

#include "monitors/collections/domain.h"
#include "monitors/collections/domain2.h"
#include <gtest/gtest.h>

class TestDomain : public testing::Test {
  protected:
    DomainStorage dom1;
    DomainStorage dom2;
    DomainStorage dom3;

    DomainStorage2 dom4;

  private:
    void SetUp() override;
};
