#pragma once
#include <string>
#include <sstream>
#include <iomanip>
namespace LinkedOut {
	class Time {
	public:
		Time() = default;

		Time(const std::string& day, const std::string& month, const std::string& year, const std::string& hour, const std::string& minute, const std::string& second)
			: m_Day(day), m_Month(month), m_Year(year), m_Hour(hour), m_Minute(minute), m_Second(second)
		{
		}


		const std::string& GetDay()const { return m_Day; }
		const std::string& GetMonth()const{return m_Month;}
		const std::string& GetYear()const{return m_Year;}
		const std::string& GetHour()const{return m_Hour;}
		const std::string& GetMinute()const{return m_Minute;}
		const std::string& GetSecond()const{return m_Second;}

		void SetDay(const std::string& Day) { m_Day = Day; }
		void SetMonth(const std::string& Month) { m_Month = Month;}
		void SetYear(const std::string& Year) { m_Year = Year;}
		void SetHour(const std::string& Hour) { m_Hour = Hour;}
		void SetMinute(const std::string& Minute) { m_Minute = Minute;}
		void SetSecond(const std::string& Second) { m_Second = Second;}


		void ExtractFromTimestamp(const std::string& timestamp) {
			std::tm tm = {}; // Initialize to all zeros

			std::stringstream ss(timestamp);
			ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

			if (ss.fail()) {
				return;
			}

			m_Year = std::to_string(tm.tm_year + 1900);
			m_Month = std::to_string(tm.tm_mon + 1);
			m_Day = std::to_string(tm.tm_mday);
			m_Hour = std::to_string(tm.tm_hour);
			m_Minute = std::to_string(tm.tm_min);
			m_Second = std::to_string(tm.tm_sec);
		}

	private:
		std::string m_Day = "0";
		std::string m_Month = "0";
		std::string m_Year = "0";
		std::string m_Hour = "0";
		std::string m_Minute = "0";
		std::string m_Second = "0";
	};
}