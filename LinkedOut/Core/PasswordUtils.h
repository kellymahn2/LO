#pragma once

#include <string>

namespace LinkedOut {

	class PasswordUtils {
	public:
		static std::string GenerateSalt();
		static std::string HashPassword(const std::string& password, const std::string& salt);
		static bool VerifyPassword(const std::string& password, const std::string& hashedPassword, const std::string& salt);
	};


}