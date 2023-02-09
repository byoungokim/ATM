#include "bank/bank_api.hpp"
#include "atm/atm_controller.hpp"
#include <gtest/gtest.h>

using bank::BankApiSharedPtr;
using bank::BankApi;
namespace atm {
TEST(AtmControllerTest, insertCard) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_EQ(controller.insertCard(), 1);
}

TEST(AtmControllerTest, checkPinNumberNegative) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_FALSE(controller.checkPinNumber(1, 1111));
  EXPECT_EQ(controller.insertCard(), 1);
  EXPECT_FALSE(controller.checkPinNumber(1, 2222));
}

TEST(AtmControllerTest, checkPinNumberPositive) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  int cardNum = controller.insertCard();
  EXPECT_TRUE(controller.checkPinNumber(1, 1111));
}

TEST(AtmControllerTest, ejectCardNegative) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);

  EXPECT_FALSE(controller.ejectCard());
}

TEST(AtmControllerTest, ejectCardPositive) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_EQ(controller.insertCard(), 1);
  EXPECT_FALSE(controller.ejectCard());
}

TEST(AtmControllerTest, showAccountsNegative) {
  AtmController controller(nullptr);
  EXPECT_EQ(controller.showAccounts().size(), 0);

  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller2(api);
  EXPECT_EQ(controller2.showAccounts().size(), 0);
}

TEST(AtmControllerTest, showAccountsPositive) {
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_EQ(controller.insertCard(),1);
  EXPECT_TRUE(controller.checkPinNumber(1, 1111));
  EXPECT_EQ(controller.showAccounts().size(), 2);
}

TEST(AtmControllerTest, getBalancePositive){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_EQ(controller.insertCard(),1);
  EXPECT_TRUE(controller.checkPinNumber(1, 1111));
  EXPECT_TRUE(controller.selectAccount("account1"));
  EXPECT_EQ(controller.getBalance(), 20);
  EXPECT_TRUE(controller.selectAccount("account2"));
  EXPECT_EQ(controller.getBalance(), 50);
}

TEST(AtmControllerTest, getBalanceNegative) {
  AtmController controller(nullptr);
  EXPECT_EQ(controller.getBalance(), -1);

  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller2(api);
  controller2.selectAccount("account3");
  EXPECT_EQ(controller2.getBalance(), -1);
}

TEST(AtmControllerTest, withdrawNegative){
  AtmController controller(nullptr);
  controller.selectAccount("account1");
  EXPECT_EQ(controller.withdraw(20), -1);

  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller2(api);
  controller2.selectAccount("account1");
  EXPECT_EQ(controller2.withdraw(30), -1);
  controller2.selectAccount("account3");
  EXPECT_EQ(controller2.withdraw(10), -1);
}

TEST(AtmControllerTest, withdrawPositive){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_EQ(controller.insertCard(),1);
  EXPECT_TRUE(controller.checkPinNumber(1, 1111));
  EXPECT_TRUE(controller.selectAccount("account1"));
  EXPECT_EQ(controller.withdraw(10), 10);
  EXPECT_EQ(controller.withdraw(10), 0);
  EXPECT_TRUE(controller.selectAccount("account2")); 
  EXPECT_EQ(controller.withdraw(10), 40);
}

TEST(AtmControllerTest, depositNegative){
  AtmController controller(nullptr);
  controller.selectAccount("account1");
  EXPECT_EQ(controller.deposit(10), -1);

  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller2(api);
  controller2.selectAccount("account3");
  EXPECT_EQ(controller2.deposit(30), -1);
}

TEST(AtmControllerTest, depositPositive){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_EQ(controller.insertCard(),1);
  EXPECT_TRUE(controller.checkPinNumber(1, 1111));
  EXPECT_TRUE(controller.selectAccount("account1"));
  EXPECT_EQ(controller.deposit(10), 30);
  EXPECT_TRUE(controller.selectAccount("account2"));
  EXPECT_EQ(controller.deposit(10), 60);
}

}
