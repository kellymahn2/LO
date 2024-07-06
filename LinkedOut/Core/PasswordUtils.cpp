#include "PasswordUtils.h"
#include <QByteArray>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QString>
namespace LinkedOut {
    std::string PasswordUtils::GenerateSalt() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        std::string salt;
        for (int i = 0; i < 16; ++i) { // 16 bytes salt
            salt += static_cast<char>(dis(gen));
        }

        // Base64 encode the salt
        QByteArray saltByteArray = QByteArray::fromRawData(salt.data(), salt.size());
        return saltByteArray.toBase64().toStdString();
    }

    std::string PasswordUtils::HashPassword(const std::string& password, const std::string& salt) {
        QByteArray saltedPassword = QByteArray::fromStdString(password) + QByteArray::fromBase64(QByteArray::fromStdString(salt));
        QByteArray hash = QCryptographicHash::hash(saltedPassword, QCryptographicHash::Sha256);
        return hash.toHex().toStdString();
    }

    bool PasswordUtils::VerifyPassword(const std::string& password, const std::string& hashedPassword, const std::string& salt) {
        std::string newHashedPassword = HashPassword(password, salt);
        return hashedPassword == newHashedPassword;
    }
}