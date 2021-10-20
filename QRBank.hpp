#pragma once

#include <string>
#include "helper/helper.hpp"

class BankAccount {
public:
    BankAccount();
    BankAccount(const std::string & initialNumber, const std::string & accountNumber, const std::string & accountName, const std::string & bankName);

    bool setInitialNumber(const std::string & newInitialNumber);
    std::string getInitialNumber() const;

    bool setAccountNumber(const std::string & newAccountNumber);
    std::string getAccountNumber() const;

    bool setAccountName(const std::string & newAccountName);
    std::string getAccountName() const;

    bool setBankName(const std::string & newBankName);
    std::string getBankName() const;

    std::string getEncodedAccountNumber() const;
    std::string getEncodedAccountName() const;

    std::string buildMessageForQR(const std::string & createdAt) const;

private:
    std::string initialNumber;
    std::string accountNumber;
    std::string accountName;
    std::string bankName;
};

class BankQR {
public:
    // static BankQR encode(string message);
    BankQR(const std::string & message);
    ~BankQR();

    bool saveToPNG(const std::string & filename) const;

    std::string toString() const;
    std::string toString(int margin) const;

    bool saveStringTo(const std::string & filename, int margin=0) const;

    static std::string decodePNG(const std::string & filename);

    static std::string decodeInfoOfMessage(const std::string & message);

    static BankQR convert(const BankQR & inQR, const std::string & targetBank);

private:
    int n;
    bool** mQr;
};