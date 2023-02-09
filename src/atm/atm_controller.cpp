/**
 * implementation of atm_controller
*/

#include "atm/atm_controller.hpp"
#include <iostream>
using namespace atm;
using namespace std;
/**
 * @brief Construct a new Atm Controller:: Atm Controller object
 * 
 * @param bankApiWrapper the bank api wrapper in a shared pointer
 */
AtmController::AtmController(bank::BankApiSharedPtr bankApiWrapper){
  this->bankApiWrapper_ =  bankApiWrapper;
  this->state_ = AtmState::BOOTING;
  if(this->checkCardSlot()) this->reportCriticalError("Atm Machine is not working due to non returned card");
  this->state_ = AtmState::WAITING;
  this->account_ = "";
}

/**
 * @brief Check the card slot
 * 
 * @return true When there is a card
 * @return false There is no card in the slot
 */
bool AtmController::checkCardSlot(){
  // mechanically check the card slot and assume that it is passed
  this->cardInserted_ = false;
  return this->cardInserted_;
}

void AtmController::reportCriticalError(string message){
  this->state_ = AtmState::ERROR;
  cerr << message << endl;
}

void AtmController::reportError(string message){
  cerr << message << endl;
}

int AtmController::countDollars(){
  this->dollars_ = 100;
  return this->dollars_;
}
/**
 * @brief Trigger the logic to insert a card
 * 
 * @return int The number of an inserted card
 */
int AtmController::insertCard(){
  this->cardInserted_ = true; // assume the card is inserted
  if(!checkCardSlot()){
    this->reportError("There is not card inserted");
  }
  int cardNum = readCardNumber();
  if(!this->bankApiWrapper_->checkCardValidity(cardNum)){
    this->reportError("The card is not usable");
    return -1;
  }
  this->state_ = AtmState::WAITING_PIN;
  return cardNum;
}
/**
 * @brief Read the card number
 * 
 * @return int The recognized card number
 */
int AtmController::readCardNumber(){
  return 1;
}
/**
 * @brief Check the validity of card number and the pin number for it
 * 
 * @param cardNum The target card number
 * @param pinNumber The pin number of a card
 * @return true if card number and pin number are valid and matched
 * @return false otherwise
 */
bool AtmController::checkPinNumber(int cardNum, int pinNumber) {
  if(this->state_ == AtmState::ERROR) return false;
  if(this->state_!= AtmState::WAITING_PIN) {
    this->reportError("Please insert a card first");
    return false;
  }
  if(!this->bankApiWrapper_) this->reportCriticalError("Please restart the machine");
  if(!this->bankApiWrapper_->checkConnection()) {
    return false; // error case
  }
  if(!this->bankApiWrapper_->checkCardValidity(cardNum))
    return false;
  this->state_ = AtmState::AUTHENTICATED;
  return this->bankApiWrapper_->checkPinNumber(cardNum, pinNumber);
}

/**
 * @brief Trigger the logic to eject a card
 * 
 */
bool AtmController::ejectCard() {
  if(!checkCardSlot()) {
    this->reportError("No card is inserted");
    return false;
  }
  // assume the card would be always ejected
  this->cardInserted_ = false;
  this->account_ = "";
  if(checkCardSlot()) {
    this->reportCriticalError("The card remains in the slot, stop all the following job for preventing the security issue");
  }
  return true;
}
/**
 * @brief Get the user confirmation on the cash box
 * 
 * @return true 
 * @return false 
 */
bool AtmController::confirmDollarsInCashDispenser(){
  // count dollars in the box and need to implement the logic to get the confirmation of a user
  // assume always true
  return true;
}

/**
 * @brief Fill the cash dispenser for a given amount of dollars
 * 
 * @param dollars The amount of withdrawl
 */
void AtmController::fillTheCashDispenser(int dollars) {
  // fill the cash dispenser
}

vector<std::string> AtmController::showAccounts(){
  if(this->state_ == AtmState::ERROR) return vector<string>{};
  if(this->state_ != AtmState::AUTHENTICATED) return vector<string>{};
  if(!this->bankApiWrapper_) {
    this->reportCriticalError("Please restart the machine");
    return vector<string>{};
  }
  if(!this->bankApiWrapper_->checkConnection()) {
    return vector<string>{}; // error case
  }
  return this->bankApiWrapper_->listAccount();
}

bool AtmController::selectAccount(std::string account){
  this->account_ = account;
  return true;
}

/**
 * @brief Deposit money
 * 
 * @param account The account number 
 * @param dollars The amount of dollar loaded on the ATM
 * @return int The balance of a given account after deposit, -1 if there is an error
 */
int AtmController::deposit(int dollars) {
  if(this->state_ == AtmState::ERROR) return -1;
  if(this->state_ != AtmState::AUTHENTICATED) return -1;
  if(this->account_.length() == 0) return -1;
  if(!this->bankApiWrapper_) {
    this->reportCriticalError("Please restart the machine");
    return -1;
  }
  if(!this->bankApiWrapper_->checkConnection()) {
    return -1; // error case
  }

  while(!this->confirmDollarsInCashDispenser()) {
    this->reportError("the user confirmation on the count is not correct");
    return -1; // error case
  }
  int balance = this->bankApiWrapper_->deposit(this->account_, dollars);
  if(balance== -1) {
    return -1;
  }
  this->dollars_ += dollars;
  return balance;
}
/**
 * @brief Returns the balance of a given account
 * 
 * @param account the account number
 * @return int the balance of a given account
 */
int AtmController::getBalance(){
  if(this->state_ == AtmState::ERROR) return -1;
  if(this->state_ != AtmState::AUTHENTICATED) return -1;
  if(this->account_.length() == 0) return -1;
  if(!this->bankApiWrapper_) {
    this->reportCriticalError("Please restart the machine");
    return -1;
  }

  int balance = this->bankApiWrapper_->getBalance(this->account_);
  if(balance < 0) { // assume there is no negative balance
    this->reportError("Account does not exist");
    return -1;
  }
  return balance;
}

/**
 * @brief Withdraw money from the account
 * 
 * @param dollars the amount of dollars to be withdrawed
 * @return int the remained balance, -1 if there is any error
 */
int AtmController::withdraw(int dollars){
  if(this->state_ == AtmState::ERROR) return -1;
  if(this->state_ != AtmState::AUTHENTICATED) return -1;
  if(this->account_.length() == 0) return -1;
  if(!this->bankApiWrapper_) {
    this->reportCriticalError("Please restart the machine");
    return -1;
  }
  if(this->dollars_ < dollars) {
    this->reportError("Not enough cash in the bin");
    return -1;
  }
  if(this->bankApiWrapper_->getBalance(this->account_) < dollars) {
    this->reportError("Not enough balance");
    return -1;
  }
  int remained = this->bankApiWrapper_->withdraw(this->account_, dollars);
  if (remained == -1){
    this->reportError("Error on the withdrawl");
    return -1;
  }

  this->fillTheCashDispenser(dollars);
  return remained;
}
