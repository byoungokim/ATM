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

TEST(AtmControllerTest, getBalancePositive){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_EQ(controller.insertCard(),1);
  EXPECT_TRUE(controller.checkPinNumber(1, 1111));
  EXPECT_EQ(controller.getBalance("account1"), 20);
  EXPECT_EQ(controller.getBalance("account2"), 50);
}

TEST(AtmControllerTest, getBalanceNegative) {
  AtmController controller(nullptr);
  EXPECT_EQ(controller.getBalance("account1"), -1);

  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller2(api);
  EXPECT_EQ(controller2.getBalance("account3"), -1);
}

TEST(AtmControllerTest, withdrawNegative){
  AtmController controller(nullptr);
  EXPECT_EQ(controller.withdraw("account1", 20), -1);

  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller2(api);
  EXPECT_EQ(controller2.withdraw("account1", 30), -1);
  EXPECT_EQ(controller2.withdraw("account3", 10), -1);
}

TEST(AtmControllerTest, withdrawPositive){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_EQ(controller.insertCard(),1);
  EXPECT_TRUE(controller.checkPinNumber(1, 1111));
  EXPECT_EQ(controller.withdraw("account1", 10), 10);
  EXPECT_EQ(controller.withdraw("account1", 10), 0);
  EXPECT_EQ(controller.withdraw("account2", 10), 40);
}

TEST(AtmControllerTest, depositNegative){
  AtmController controller(nullptr);
  EXPECT_EQ(controller.deposit("account1", 10), -1);

  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller2(api);
  EXPECT_EQ(controller2.deposit("account3", 30), -1);
}

TEST(AtmControllerTest, depositPositive){
  BankApiSharedPtr api = std::make_shared<BankApi>();
  AtmController controller(api);
  EXPECT_EQ(controller.insertCard(),1);
  EXPECT_TRUE(controller.checkPinNumber(1, 1111));
  EXPECT_EQ(controller.deposit("account1", 10), 30);
  EXPECT_EQ(controller.deposit("account2", 10), 60);
}

}
