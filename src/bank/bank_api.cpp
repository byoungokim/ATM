/**
 * wrapping functions of Bank Api 
*/
#include "bank/bank_api.hpp"
using namespace bank;

BankApi::BankApi(){
  this->accounts["account1"] = 20;
  this->accounts["account2"] = 50;
}

bool BankApi::checkConnection(){
  return true;
}

bool BankApi::checkCardValidity(int cardNum) {
  return cardNum == 1;
}

bool BankApi::checkPinNumber(int cardNum, int pinNum) {
  if(cardNum == 1 && pinNum == 1111) return true; // assume there is a card whose number and pin number are 1 and 1111, respectively
  return false;
}

vector<string> BankApi::listAccount(){
  // assume it will return account1 and account2
  return vector<string>{"account1","account2"};
}

int BankApi::getBalance(string account) {
  if(this->accounts.find(account) != this->accounts.end()) {
    return this->accounts[account];
  }
  return -1;
}

int BankApi::deposit(string account, int dollars){
  if(this->accounts.find(account) !=this->accounts.end()) {
    this->accounts[account]+=dollars;
    return this->accounts[account];
  }
  return -1;
}

int BankApi::withdraw(string account, int dollars) {
  if(this->accounts.find(account) != this->accounts.end() && this->accounts[account]>= dollars) {
    this->accounts[account]-=dollars;
    return this->accounts[account];
  }
  return -1;
}
