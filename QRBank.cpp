#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>

#include "QRBank.hpp"
BankAccount::BankAccount() {
	this->initialNumber = "";
	this->accountNumber = "";
	this->accountName = "";
	this->bankName = "";
}

BankAccount::BankAccount(const std::string & initialNumber, const std::string & accountNumber, 
	const std::string & accountName, const std::string & bankName) {
	this->initialNumber = initialNumber;
	this->accountNumber = accountNumber;
	this->accountName = accountName;
	this->bankName = bankName;
}
// hàm check có đúng là ký tự số
bool isNumber(char c) { 
	return c >= '0' && c <= '9';
}
// hàm check có đúng ký tự in hoa
bool isUpper(char c) {
	return c >= 'A' && c <= 'Z';
}
// hàm check ký tự thường
bool isLower(char c) {
	return c >= 'a' && c <= 'z';
}

bool BankAccount::setInitialNumber(const std::string & newInitialNumber) {
	int len = (int) newInitialNumber.size();
	for(int i = 0; i < (int) newInitialNumber.size(); i++) {
		if(!isNumber(newInitialNumber[i])) {
			return false;
		}
	}
	this->initialNumber = newInitialNumber;
	return true;
}

std::string BankAccount::getInitialNumber() const {
	return this->initialNumber;
}

bool BankAccount::setAccountNumber(const std::string & newAccountNumber) {
	int len = (int) newAccountNumber.size();
	int lenBN = (int) this->bankName.size();
	if(lenBN > 0) {
		if(this->bankName == "VCBKB") {
			if(len != 13) {
				return false;
			}
		} else if(bankName == "OCBKB") {
			if(len != 15) {
				return false;
			}
		} else if(bankName == "ACBKB") {
			if(len != 8) {
				return false;
			}
		}
	}
	if(len < 8 || len > 15) return false; // thoa điều kiện 8-15 số
	for(int i = 0; i < len; i++) { 
		if(!isNumber(newAccountNumber[i])) {
			return false;
		}
	}
	this->accountNumber = newAccountNumber;
	return true;
}

std::string BankAccount::getAccountNumber() const {
	return this->accountNumber;
}

bool BankAccount::setAccountName(const std::string & newAccountName) {
	int len = (int) newAccountName.size();
	if(len > 30 || len == 0) return false; // tên tài khoản tối đa 30 chữ
	if(!isUpper(newAccountName[0]) || isUpper(newAccountName[len - 1])) return false;
	// chữ đầu phải viết hoa, chữ kết thúc phải ghi thường
	for(int i = 0; i < len; i++) {
		if(!isLower(newAccountName[i]) && !isUpper(newAccountName[i])) {
			return false;
			// vị trí trong string này ko đúng case thường và hoa - khoảng trắng
		}
		if(i < len - 1 && isUpper(newAccountName[i]) && isUpper(newAccountName[i + 1])) {
			return false;
			// 2 chữ cái liền kề ko dc cùng viết Hoa
		}
	}
	this->accountName = newAccountName;
	return true;
}

std::string BankAccount::getAccountName() const {
	return this->accountName;
}

bool BankAccount::setBankName(const std::string & newBankName) {
	int len = (int) this->accountNumber.size();
	if(newBankName == "VCBKB") {
		if(len == 0 || len == 13) { 
			this->bankName = newBankName;
			return true;
		}
	} else if(newBankName == "OCBKB") {
		if(len == 0 || len == 15) {
			this->bankName = newBankName;
			return true;
		}
	} else if(newBankName == "ACBKB") {
		if(len == 0 || len == 8) {
			this->bankName = newBankName;
			return true;
		}
	}
	return false;
}

int toInt(char c) {
	return c - '0';
}

// a - '0' is equivalent to ((int)a) - ((int)'0'), 
// which means the ascii values of the characters are subtracted from each other. 
// Since 0 comes directly before 1 in the ascii table (and so on until 9), 
// the difference between the two gives the number that the character a represents.

std::string BankAccount::getBankName() const {
	return this->bankName;
}

std::string BankAccount::getEncodedAccountNumber() const {
	//std::cout << this->accountNumber << std::endl;
	// find k
	int s = 0;
	int len = (int) this->accountNumber.size();
	for(int i = 0; i < len; i++) {
		s += toInt(this->accountNumber[i]);
	}
	//std::cout << "S: " << s << std::endl;
	int f = s % 3 + 3;
	//std::cout << "F: " << f << std::endl;
	std::string d = this->accountNumber;
	int lenD = len; // biến tạm của len là lenD dùng để tính d
	while((int) d.size() % f != 0) { 
		// Chia số tài khoản thành từng đoạn có độ dài là f
		++lenD;
		d = d[0] + d;
	}
	//std::cout << "D: " << d << std::endl;
	int k = 0;
	for(int i = 0; i < lenD; i += f) {
		int curr = 0;
		for(int j = i; j < i + f; j++) {
			curr = curr * 10 + toInt(d[j]);
		}
		//std::cout << "CURR: " << curr << std::endl;
		k += curr;
	}
	//std::cout << "K: " << k << std::endl;
	int lK = 0, dK = k;
	std::string sk = "";
	while(dK > 0) {
		char c = (dK % 10) + '0';
		++lK;
		dK /= 10;
		sk = c + sk;
	}
	//std::cout << "LK: " << lK << std::endl;
	//std::cout << "SK: " << sk << std::endl;

	//BUILD VIGENERE MATRIX
	char vigc[10][10];
	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
			vigc[i][j] = ((i + j) % 10 + '0');
		}
	}
	// for(int i = 0; i < 10; i++) {
	// 	for(int j = 0; j < 10; j++) {
	// 		std::cout << vigc[i][j] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	
	std::string ans = "";
	for(int i = 0; i < len; i++) {
		int j = i % lK;
		// std::cout << "J: " << j << std::endl;
		ans = ans + vigc[toInt(sk[j])][toInt(this->accountNumber[i])];
	}
	return ans;
}

bool isPrime(int n) {
	for(int i = 2; i * i <= n; i++) {
		if(n % i == 0) return false;
	}
	return true;
}

int greatestPrimeLessThan(int n) {
	for(int i = n; ; i++) {
		if(isPrime(i)) {
			return i;
		}
	}
	return -1;
}

std::string BankAccount::getEncodedAccountName() const {
	std::string encodedAccountNumber = this->getEncodedAccountNumber();
	int s = 0;
	int len = (int) encodedAccountNumber.size();
	for(int i = 0; i < len; i++) {
		s += toInt(encodedAccountNumber[i]);
	}
	//std::cout << "S: " << s << std::endl;
	int p = greatestPrimeLessThan(s);
	//std::cout << "P: " << p << std::endl;
	int d = p % 52;
	//std::cout << "D: " << d << std::endl;
	std::string ab = ""; // chuỗi string của Virgenere để encrypt cùng
	for(char c = 'a'; c <= 'z'; c++) {
		int u = c - 'a';
		char p = 'A' + u;
		ab = ab + p;
		ab = ab + c;
	}
	//std::cout << "AB: " << ab << std::endl;
	std::string ans = "";
	int lenAN = (int) this->accountName.size();
	//std::cout << this->accountName << std::endl;
	for(int i = 0; i < lenAN; i++) {
		for(int j = 0; j < 52; j++) {
			if(ab[j] == this->accountName[i]) {
				int nI = (j + d) % 52;
				ans = ans + ab[nI];
				break;
			}
		}
	}
	// std::cout << ans << std::endl;
	return ans;
}

std::string BankAccount::buildMessageForQR(const std::string & createdAt) const {
	std::string dd = "", mm = "", yyyy = "", curr = "";
	int lca = (int) createdAt.size();
	for(int i = 0; i < lca; i++) {
		if(createdAt[i] == '-') {
			if(dd == "") dd = curr;
			else if(mm == "") mm = curr;
			curr = "";
		} else curr = curr + createdAt[i];
	}
	yyyy = curr;
	// std::cout << this->getEncodedAccountName() << std::endl;
	if(this->bankName == "VCBKB") {
		return this->initialNumber + this->bankName + this->getEncodedAccountNumber() + this->getEncodedAccountName() + dd + mm + yyyy;
	} else if(bankName == "OCBKB") {
		return this->initialNumber + mm + dd + yyyy + this->bankName + this->getEncodedAccountNumber() + this->getEncodedAccountName();
	} else if(bankName == "ACBKB") {
		return this->initialNumber + this->getEncodedAccountName() + this->getEncodedAccountNumber() + yyyy[2] + yyyy[3] + mm + dd + this->bankName;
	}
	return "@@@";
}

BankQR::BankQR(const std::string & message) {
	this->mQr = encodeQR(message, this->n);
}

BankQR::~BankQR() {
	for(int i = 0; i < this->n; i++) {
		delete[] mQr[i];
	}
	delete[] mQr;
}

bool BankQR::saveToPNG(const std::string & filename) const {
	return writeQRToPNG(this->mQr, this->n, filename);
}

std::string BankQR::toString() const {
	std::string ans = "";
	for(int i = 0; i < this->n; i++) {
		for(int j = 0; j < this->n; j++) {
			if(!this->mQr[i][j]) {
				ans = ans + "  ";
			} else ans = ans + "##";
		}
		if(i != this->n - 1) ans = ans + '\n';
	}
	return ans;
}

std::string BankQR::toString(int margin) const {
	std::string ans = "";
	ans += "+";	// điểm bắt đầu tại các góc vuông
	for(int i = 0; i < margin; i++) ans = ans + "-"; // vẽ đường gach - cho lề theo chiều dọc (xét i so với margin )
	for(int i = 0; i < this->n; i++) ans = ans + "--"; // 
	for(int i = 0; i < margin; i++) ans = ans + "-";
	ans += "+\n";
	for(int i = 0; i < margin; i++) {
		ans = ans + "|";
		for(int j = 0; j < margin; j++) ans = ans + " ";
		for(int j = 0; j < this->n; j++) ans = ans + "  ";
		for(int j = 0; j < margin; j++) ans = ans + " ";
		ans = ans + "|";
		ans = ans + '\n';
	}
	for(int i = 0; i < this->n; i++) {
		ans = ans + "|";
		for(int j = 0; j < margin; j++) ans = ans + " ";
		for(int j = 0; j < this->n; j++) {
			if(!this->mQr[i][j]) {
				ans = ans + "  ";
			} else ans = ans + "##";
		}
		for(int j = 0; j < margin; j++) ans = ans + " ";
		ans = ans + "|";
		ans = ans + '\n';
	}
	for(int i = 0; i < margin; i++) {
		ans = ans + "|";
		for(int j = 0; j < margin; j++) ans = ans + " ";
		for(int j = 0; j < this->n; j++) ans = ans + "  ";
		for(int j = 0; j < margin; j++) ans = ans + " ";
		ans = ans + "|";
		ans = ans + '\n';
	}
	ans += "+";
	for(int i = 0; i < margin; i++) ans = ans + "-";
	for(int i = 0; i < this->n; i++) ans = ans + "--";
	for(int i = 0; i < margin; i++) ans = ans + "-";
	ans += "+\n";
	return ans;
}

bool BankQR::saveStringTo(const std::string & filename, int margin) const {
	std::ofstream out(filename);
	if(out.fail()) return false;
	out << this->toString(margin);
	out.close();
	return true;
}

/*std::string BankQR::decodePNG(const std::string & filename) {
  cv::Mat src = cv::imread(filename);
  if(src.empty()) {
  	return "[Error]: Can not open " + filename;
  }
	cv::QRCodeDetector qrDecoder = cv::QRCodeDetector();
	cv::Mat bbox, rectifiedImage;
  std::string data = qrDecoder.detectAndDecode(src, bbox, rectifiedImage);
  if(data.length() > 0) {
  	return data;
	}
	return "[Error]: QR Code not detected";
}
*/
// BankQR BankQR::convert(const BankQR & inQR, const std::string & targetBank) {
// 	std::string pngTmpFile = "tmpQR.png";
//   inQR.saveToPNG(pngTmpFile);
//   std::ifstream nameFileout;
//   std::string message = decodePNG(pngTmpFile);
//   // std::string message = "00020101021nNnTGlBXG23696703210831ACBKB";
//   std::string infos = inQR.decodeInfoOfMessage(message);
//   // std::cout << infos << std::endl;
//   std::string bankName = "", accountNumber = "", accountName = "", createdAt = "", initialNumber = "";
//   int curr = 0;
//   while(infos[curr] != ':') ++curr;
//   while(infos[curr] != '\n') {
//     if(infos[curr] != ' ' && infos[curr] != ':') bankName += infos[curr];
//     ++curr;
//   }
//   // std::cout << "Bank name: " << bankName << std::endl;
//   while(infos[curr] != ':') ++curr;
//   while(infos[curr] != '\n') {
//     if(infos[curr] != ' ' && infos[curr] != ':') accountNumber += infos[curr];
//     ++curr;
//   }
//   // std::cout << "Account number: " << accountNumber << std::endl;
//   while(infos[curr] != ':') ++curr;
//   while(infos[curr] != '\n') {
//     if(infos[curr] != ' ' && infos[curr] != ':') accountName += infos[curr];
//     ++curr;
//   }
//   // std::cout << "Account name: " << accountName << std::endl;
//   infos = infos + '\n';
//   while(infos[curr] != ':') ++curr;
//   while(infos[curr] != '\n') {
//     if(infos[curr] != ' ' && infos[curr] != ':') createdAt += infos[curr];
//     ++curr;
//   }
//   // std::cout << "Created At: " << createdAt << std::endl;
//   std::string dd = "";
//   dd += createdAt[0];
//   dd += createdAt[1];
//   std::string mm = "";
//   mm += createdAt[3]; 
//   mm += createdAt[4];
//   std::string yyyy = "";
//   yyyy += createdAt[6];
//   yyyy += createdAt[7];
//   yyyy += createdAt[8];
//   yyyy += createdAt[9];
//   std::string yy = "";
//   yy += createdAt[8];
//   yy += createdAt[9];
//   // std::cout << dd << " " << mm << " " << yyyy << " " << yy << std::endl;
//   std::string newMessage = "@@@";
//   if(targetBank == "VCBKB") {
//     curr = 0;
//     while(isNumber(message[curr])) {
//       initialNumber += message[curr];
//       ++curr;
//     }
//     newMessage = initialNumber + targetBank + accountNumber + accountName + dd + mm + yyyy;
//   } else if (targetBank == "OCBKB") {
//     curr = (int) message.size() - 1;
//     while(isUpper(message[curr]) || isLower(message[curr])) --curr;
//     while(isNumber(message[curr])) --curr;
//     while(isUpper(message[curr])) --curr;
//     curr = curr - 8;
//     for(int i = 0; i <= curr; i++) initialNumber += message[i];
//     newMessage = initialNumber + mm + dd + yyyy + targetBank + accountNumber + accountName;
//   } else if (targetBank == "ACBKB") {
//     curr = 0;
//     while(isNumber(message[curr])) {
//     	initialNumber += message[curr];
//       ++curr;
//     }
//     newMessage = initialNumber + accountName + accountNumber + yy + mm + dd + targetBank;
//   }

//   BankQR bankQr(newMessage);
//   return bankQr;
// }

std::string BankQR::decodeInfoOfMessage(const std::string & message) {
	// std::cout << message << std::endl;
	// std::cout << "Decode info of message " << std::endl;
  int l = (int) message.size();
  std::string bankName = "", accountNumber = "", accountName = "", createdAt = "";
  if(message.substr(l - 5) == "ACBKB") {
  	// std::cout << "GO ACBKB" << std::endl;
    // ACBKB
    int curr = 0;
    while(isNumber(message[curr])) ++curr;
    while(isUpper(message[curr]) || isLower(message[curr])) {
      accountName += message[curr];
      ++curr;
    }
    // std::cout << "Account name: " << accountName << std::endl;
    // std::cout << "L = " << l << " . Curr = " << curr << std::endl; 
    for(int i = curr; i < curr + 8; i++) {
      accountNumber += message[i];
    }
    curr += 8;
    // std::cout << "Account number: " << accountNumber << std::endl;
    std::string yyyy = "20";
    yyyy += message[curr];
    yyyy += message[curr + 1];
    std::string mm = "";
    mm += message[curr + 2]; 
    mm += message[curr + 3];
    std::string dd = "";
    dd += message[curr + 4];
    dd += message[curr + 5];
    bankName = "ACBKB";
    return "Ngan hang: " + bankName + "\nSo tai khoan: " + accountNumber + "\nTen chu tai khoan: " + accountName + "\nThoi diem tao thong diep QR: " + dd + "/" + mm + "/" + yyyy;
  } 
  int curr = l - 1;
  while(isUpper(message[curr]) || isLower(message[curr])) --curr;
  while(isNumber(message[curr])) --curr;
  while(isUpper(message[curr])) --curr;
  if(message.substr(curr + 1, 5) == "OCBKB") {
    // OCBKB
    std::string rBankName = "", rAccountNumber = "", rAccountName = "";
    curr = l - 1;
    while(isUpper(message[curr]) || isLower(message[curr])) {
      rAccountName += message[curr];
      --curr;
    }
    while(isNumber(message[curr])) {
      rAccountNumber += message[curr];
      --curr;
    }
    while(isUpper(message[curr])) --curr;
    std::string yyyy = "";
    yyyy += message[curr - 3];
    yyyy += message[curr - 2];
    yyyy += message[curr - 1];
    yyyy += message[curr];
    std::string mm = "";
    mm += message[curr - 5];
    mm += message[curr - 4];
    std::string dd = "";
    dd += message[curr - 7];
    dd += message[curr - 6];
    for(int i = 0; i < (int) rBankName.size(); i++) bankName += rBankName[i];
    for(int i = 0; i < (int) rAccountNumber.size(); i++) accountNumber += rAccountNumber[i];
    for(int i = 0; i < (int) rAccountName.size(); i++) accountName += rAccountName[i];
    bankName = "OCBKB";
    return "Ngan hang: " + bankName + "\nSo tai khoan: " + accountNumber + "\nTen chu tai khoan: " + accountName + "\nThoi diem tao thong diep QR: " + dd + "/" + mm + "/" + yyyy;
  } 
  // VCBKB
  curr = l - 1;
  std::string yyyy = "";
  yyyy += message[curr - 3];
  yyyy += message[curr - 2];
  yyyy += message[curr - 1];
  yyyy += message[curr];
  std::string mm = "";
  mm += message[curr - 5];
  mm += message[curr - 4];
  std::string dd = "";
  dd += message[curr - 7];
  dd += message[curr - 6];
  // std::cout << dd << std::endl;
  // std::cout << mm << std::endl;
  // std::cout << yyyy << std::endl;
  curr = 0;
  while(isNumber(message[curr])) ++curr;
  while(isUpper(message[curr])) {
  	bankName += message[curr];
  	++curr;
  }
  while(isNumber(message[curr])) {
    accountNumber += message[curr];
    ++curr;
  }
  while(isUpper(message[curr]) || isLower(message[curr])) {
    accountName += message[curr];
    ++curr;
  }
  return "Ngan hang: " + bankName + "\nSo tai khoan: " + accountNumber + "\nTen chu tai khoan: " + accountName + "\nThoi diem tao thong diep QR: " + dd + "/" + mm + "/" + yyyy;
}

int main (){
	BankAccount *ba = new BankAccount("00020101021", "012100002512112", "VoThiQuynhNhu", "OCBKB");
  	std::cout << ba->buildMessageForQR("22-08-2021") << std::endl;
  	std::cout << BankQR::decodeInfoOfMessage("00020101021VCBKB2159659309374cCcIVaQMV01092021") << std::endl;
	BankQR * bankQr = new BankQR(ba->buildMessageForQR("22-08-2021"));
	bankQr->saveToPNG("nqr.png");
	std::cout << bankQr->toString(2) << std::endl;
	bankQr->saveStringTo("out.txt", 2);
	
	// BankQR bankQr2("00020101021VCBKB2159659309374cCcIVaQMV01092021");
	// BankQR bankQr3(ba->buildMessageForQR("31-08-2021"));
	
	// BankQR::convert(bankQr, "VCBKB");
	// std::cout << bankQr->toString(0) << std::endl;
	// std::cout << BankQR::decodePNG("nqr.png") << std::endl;
	// std::cout << BankQR::decodePNG("nqr.png") << std::endl;
	return 0;
}

