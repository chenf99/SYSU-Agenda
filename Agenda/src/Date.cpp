#include "Date.hpp"
#include <sstream>

/**
* @brief default constructor
*/
Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}

/**
* @brief constructor with arguments
*/
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}

/**
* @brief constructor with a string
*/
Date::Date(const std::string &dateString) {
	*this = stringToDate(dateString);
}
/**
* @brief return the year of a Date
* @return a integer indicate the year of a date
*/
int Date::getYear(void) const{return m_year;}

/**
* @brief set the year of a date
* @param a integer indicate the new year of a date
*/
void Date::setYear(const int t_year) {
	m_year = t_year;
}

/**
* @brief return the month of a Date
* @return a integer indicate the month of a date
*/
int Date::getMonth(void) const{return m_month;}

/**
* @brief set the month of a date
* @param a integer indicate the new month of a date
*/
void Date::setMonth(const int t_month) {
	m_month = t_month;
}

/**
* @brief return the day of a Date
* @return a integer indicate the day of a date
*/
int Date::getDay(void) const{return m_day;}

/**
* @brief set the day of a date
* @param a integer indicate the new day of a date
*/
void Date::setDay(const int t_day) {
	m_day = t_day;
}

/**
* @brief return the hour of a Date
* @return a integer indicate the hour of a date
*/
int Date::getHour(void) const{return m_hour;}

/**
* @brief set the hour of a date
* @param a integer indicate the new hour of a date
*/
void Date::setHour(const int t_hour) {
	m_hour = t_hour;
}

/**
* @brief return the minute of a Date
* @return a integer indicate the minute of a date
*/
int Date::getMinute(void) const{return m_minute;}

/**
* @brief set the minute of a date
* @param a integer indicate the new minute of a date
*/
void Date::setMinute(const int t_minute) {
	m_minute = t_minute;
}

/**
* @brief check whether the date is valid or not
* @return the bool indicate valid or not
*/
int daysOfMonth(const int &year, const int &month) {
	if (year % 400 == 0 || year % 4 == 0 && year % 100 != 0) {
		int days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		return days[month - 1];
	}
	else {
		int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		return days[month - 1];
	}
}
bool Date::isValid(const Date &t_date) {
	if (t_date.getYear() < 1000 || t_date.getYear() > 9999) return false;
	if (t_date.getMonth() < 1 || t_date.getMonth() > 12) return false;
	if (t_date.getMinute() < 0 || t_date.getMinute() > 59) return false;
	if (t_date.getHour() < 0 || t_date.getHour() > 23) return false;
	if (t_date.getDay() < 1 || t_date.getDay() > daysOfMonth(t_date.getYear(), t_date.getMonth())) return false;
	return true;
}

/**
* @brief convert a string to date, if the format is not correct return
* 0000-00-00/00:00
* @return a date
*/
Date Date::stringToDate(const std::string &t_dateString) {
	if (t_dateString.length() != 16) return Date();
	if (t_dateString[4] != '-' || t_dateString[7] != '-' || t_dateString[10] != '/' || t_dateString[13] != ':') return Date();
	for (int i = 0; i < t_dateString.length(); ++i) {
		if ((t_dateString[i] < '0' || t_dateString[i] > '9') && (t_dateString[i] != '-' 
			&& t_dateString[i] != '/' && t_dateString[i] != ':')) return Date();
	}
	int year, month, day, hour, minute;
	std::string temp;
	std::stringstream ss;
	temp = t_dateString.substr(0, 4);
	ss << temp;
	ss >> year;
	ss.clear();
	temp = t_dateString.substr(5, 2);
	ss << temp;
	ss >> month;
	ss.clear();
	temp = t_dateString.substr(8, 2);
	ss << temp;
	ss >> day;
	ss.clear();
	temp = t_dateString.substr(11, 2);
	ss << temp;
	ss >> hour;
	ss.clear();
	temp = t_dateString.substr(14, 2);
	ss << temp;
	ss >> minute;
	return Date(year, month, day, hour, minute);
}

/**
* @brief convert a date to string, if the format is not correct return
* 0000-00-00/00:00
*/
std::string Date::dateToString(const Date &t_date) {
	if (isValid(t_date)) {
		std::string result;
		std::stringstream ss;
		std::string temp;
		ss << t_date.getYear();
		ss >> temp;
		result += temp;
		result += "-";
		ss.clear();
		ss << t_date.getMonth();
		ss >> temp;
		if (t_date.getMonth() < 10) result += "0";
		result += temp;
		result += "-";
		ss.clear();
		ss << t_date.getDay();
		ss >> temp;
		if (t_date.getDay() < 10) result += "0";
		result += temp;
		result += "/";
		ss.clear();
		ss << t_date.getHour();
		ss >> temp;
		if (t_date.getHour() < 10) result += "0";
		result += temp;
		result += ":";
		ss.clear();
		ss << t_date.getMinute();
		ss >> temp;
		if (t_date.getMinute() < 10) result += "0";
		result += temp;
		return result;
	}
	else {
		return "0000-00-00/00:00";
	}
}

/**
*@brief overload the assign operator
*/
Date &Date::operator=(const Date &t_date) {
	m_year = t_date.m_year;
	m_month = t_date.m_month;
	m_day = t_date.m_day;
	m_hour = t_date.m_hour;
	m_minute = t_date.m_minute;
	return *this;
}

/**
* @brief check whether the CurrentDate is equal to the t_date
*/
bool Date::operator==(const Date &t_date) const{
	return m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day 
	&& m_hour == t_date.m_hour && m_minute == t_date.m_minute;
}

/**
* @brief check whether the CurrentDate isgreater than the t_date
*/
bool Date::operator>(const Date &t_date) const{
	if (m_year > t_date.m_year) return true;
	if (m_year < t_date.m_year) return false;
	if (m_month > t_date.m_month) return true;
	if (m_month < t_date.m_month) return false;
	if (m_day > t_date.m_day) return true;
	if (m_day < t_date.m_day) return false;
	if (m_hour > t_date.m_hour) return true;
	if (m_hour < t_date.m_hour) return false;
	if (m_minute > t_date.m_minute) return true;
	return false;
}

/**
* @brief check whether the CurrentDate isless than the t_date
*/
bool Date::operator<(const Date &t_date) const{
	return !(*this > t_date || *this == t_date);
}

/**
* @brief check whether the CurrentDate isgreater or equal than the t_date
*/
bool Date::operator>=(const Date &t_date) const{
	return *this > t_date || *this == t_date;
}

/**
* @brief check whether the CurrentDate isless than or equal to the t_date
*/
bool Date::operator<=(const Date &t_date) const{
	return !(*this > t_date);
}