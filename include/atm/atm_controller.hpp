/**
 * This class is generated for controlling ATM machine
 * @author Byoungoh Kim
*/
#ifndef ATM_ATM_CONTROLLER_HPP_
#define ATM_ATM_CONTROLLER_HPP_
#include "bank/bank_api.hpp"
#include <memory>
#include <string>

namespace atm{
enum AtmState {
  BOOTING,
  WAITING,
  WAITING_PIN,
  AUTHENTICATED,
  CASH_BIN_EMPTY,
  ERROR,
};
class AtmController{
private:
  bank::BankApiSharedPtr bankApiWrapper_;
  bool cardInserted_;

  int state_;
  int dollars_;
  bool checkCardSlot();
  int readCardNumber();
  void reportCriticalError(std::string message);
  int countDollars();
  bool confirmDollarsInCashDispenser();
  void fillTheCashDispenser(int dollars);
  void reportError(std::string message);
public:
  AtmController(bank::BankApiSharedPtr bankApiWrapper);
  int insertCard();
  bool checkPinNumber(int cardNum, int pinNumber);
  bool ejectCard();
  int getBalance(std::string account);
  int deposit(std::string account, int dollars);
  int withdraw(std::string account, int dollars);
};
}
#endif // ATM_ATM_CONTROLLER_HPP_
