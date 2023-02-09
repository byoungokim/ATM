#ifndef BANK_BANK_API_HPP_
#define BANK_BANK_API_HPP_
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace bank {
using namespace std;
class BankApi{
private:
  /**
   * Assume all account balances are positive. No minus withdraw is accepted
   */
  std::unordered_map<string,int> accounts;
public:
  BankApi();
  bool checkConnection();
  bool checkCardValidity(int cardNum);
  bool checkPinNumber(int cardNum, int pinNum);
  vector<string> listAccount();
  int getBalance(string account);
  int deposit(string account, int dollars);
  int withdraw(string account, int dollars);
};

typedef shared_ptr<BankApi> BankApiSharedPtr;
}

#endif // BANK_BANK_API_HPP_
