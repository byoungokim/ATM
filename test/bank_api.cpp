#include "bank/bank_api.hpp"
#include <gtest/gtest.h>
namespace bank{

TEST(BankApiTest, checkConnection) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_TRUE(api->checkConnection());
}

TEST(BankApiTest, checkCardValidityPositive) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_TRUE(api->checkCardValidity(1));
}

TEST(BankApiTest, checkCardValidityNegative) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_FALSE(api->checkCardValidity(2));
}

TEST(BankApiTest, ccheckPinNumberPositive) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_TRUE(api->checkPinNumber(1, 1111));
}

TEST(BankApiTest, checkPinNumberNegative) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_FALSE(api->checkPinNumber(1, 0000));
}

TEST(BankApiTest, listAccount) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  vector<string> accounts = api->listAccount();
  EXPECT_EQ(accounts.size(), 2);
}

TEST(BankApiTest, getBalance) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_EQ(api->getBalance("account1"), 20);
  EXPECT_EQ(api->getBalance("account2"), 50);
}

TEST(BankApiTest, depositPositive){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_EQ(api->getBalance("account1"), 20);
  EXPECT_EQ(api->deposit("account1", 10), 30);
}

TEST(BankApiTest, depositNegative){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_EQ(api->getBalance("account3"), -1);
}

TEST(BankApiTest, withdrawPositive){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_EQ(api->getBalance("account1"), 20);
  EXPECT_EQ(api->withdraw("account1", 10), 10);
}

TEST(BankApiTest, withdrawNegative){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  EXPECT_EQ(api->withdraw("account1", 30), -1);
  EXPECT_EQ(api->withdraw("account3", 10), -1);
}

}
