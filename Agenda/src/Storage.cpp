#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
#include <vector>
#include <iostream>
using std::ifstream;
using std::ofstream;

std::shared_ptr<Storage> Storage::m_instance = nullptr;
std::shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == nullptr) {
		m_instance = std::shared_ptr<Storage>(new Storage);
	}
	return m_instance;
}

Storage::Storage() {
	m_dirty = false;
	readFromFile();
}

/**
* read file content into memory
* @return if success, true will be returned
*/
bool Storage::readFromFile(void) {
	ifstream file1(Path::userPath), file2(Path::meetingPath);
	if (file1.is_open() && file2.is_open()) {
		std::string txt1;
		while (getline(file1, txt1)) {
			for (auto it = txt1.begin(); it != txt1.end();) {
				if (*it == '"') it = txt1.erase(it);
				else ++it;
			}
			std::string username, password, email, phone;
			username = txt1.substr(0, txt1.find_first_of(','));
			txt1.erase(0, txt1.find_first_of(',') + 1);
			password = txt1.substr(0, txt1.find_first_of(','));
			txt1.erase(0, txt1.find_first_of(',') + 1);
			email = txt1.substr(0, txt1.find_first_of(','));
			txt1.erase(0, txt1.find_first_of(',') + 1);
			phone = txt1;
			m_userList.push_back(User(username, password, email, phone));
		}
		file1.close();

		std::string txt2;
		while (getline(file2, txt2)) {
			for (auto it = txt2.begin(); it != txt2.end();) {
				if (*it == '"') it = txt2.erase(it);
				else ++it;
			}
			std::string sponsor, participator, startTime, endTime, title;
			std::vector<std::string> participators;
			sponsor = txt2.substr(0, txt2.find_first_of(','));
			txt2.erase(0, txt2.find_first_of(',') + 1);
			participator = txt2.substr(0, txt2.find_first_of(','));
			txt2.erase(0, txt2.find_first_of(',') + 1);
			startTime = txt2.substr(0, txt2.find_first_of(','));
			txt2.erase(0, txt2.find_first_of(',') + 1);
			endTime = txt2.substr(0, txt2.find_first_of(','));
			txt2.erase(0, txt2.find_first_of(',') + 1);
			title = txt2;
			while (participator.find_first_of('&') != -1) {
				std::string person = participator.substr(0, participator.find_first_of('&'));
				participators.push_back(person);
				participator.erase(0, participator.find_first_of('&') + 1);
			}
			participators.push_back(participator);
			Date start = Date::stringToDate(startTime), end = Date::stringToDate(endTime);
			m_meetingList.push_back(Meeting(sponsor, participators, start, end, title));
		}
		file2.close();
		return true;
	}
	else return false;
}

/**
* write file content from memory
* @return if success, true will be returned
*/
bool Storage::writeToFile(void) {
	ofstream file1(Path::userPath), file2(Path::meetingPath);
	if (m_userList.empty() && m_meetingList.empty()) return false;
	if (file1.is_open() && file2.is_open()) {
		for (auto i:m_userList) {
			std::string txt = "\"";
			txt += i.getName();
			txt += "\",\"";
			txt += i.getPassword();
			txt += "\",\"";
			txt += i.getEmail();
			txt += "\",\"";
			txt += i.getPhone();
			txt += "\"";
			file1 << txt << "\n";
		}
		for (auto i:m_meetingList) {
			std::string txt = "\"";
			txt += i.getSponsor();
			txt += "\",\"";
			std::vector<std::string> participators = i.getParticipator();
			for (auto it = participators.begin(); it != participators.end(); ++it) {
				txt += *it;
				if (*it != participators.back()) txt += "&";
			}
			txt += "\",\"";
			Date start = i.getStartDate(), end = i.getEndDate();
			txt += Date::dateToString(start);
			txt += "\",\"";
			txt += Date::dateToString(end);
			txt += "\",\"";
			txt += i.getTitle();
			txt += "\"";
			file2 << txt << "\n";
		}
		m_dirty = false;
		file1.close();
		file2.close();
		return true;
	}
	else return false;
}

/**
* destructor
*/
Storage::~Storage() {
	sync();
	m_instance = nullptr;
}

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/**
* create a user
* @param a user object
*/
void Storage::createUser(const User &t_user) {
	m_userList.push_back(t_user);
	m_dirty = true;
}

/**
* query users
* @param a lambda function as the filter
* @return a list of fitted users
*/
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const{
	std::list<User> result;
	for (auto i:m_userList) {
		if (filter(i)) result.push_back(i);
	}
	return result;
}
/**
* update users
* @param a lambda function as the filter
* @param a lambda function as the method to update the user
* @return the number of updated users
*/
int Storage::updateUser(std::function<bool(const User &)> filter,
 std::function<void(User &)> switcher) {
	int result = 0;
	for (auto it = m_userList.begin(); it != m_userList.end(); ++it) {
		if (filter(*it)) {
			switcher(*it);
			result++;
		}
	}
	if (result) m_dirty = true;
	return result;
}

/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int result = 0;
	for (auto it = m_userList.begin(); it != m_userList.end();) {
		if (filter(*it)) {
			it = m_userList.erase(it);
			result++;
		}
		else ++it;
	}
	if (result) m_dirty = true;
	return result;
}

/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting &t_meeting) {
	m_meetingList.push_back(t_meeting);
	m_dirty = true;
}

/**
* query meetings
* @param a lambda function as the filter
* @return a list of fitted meetings
*/
std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const{
	std::list<Meeting> result;
	for (auto i:m_meetingList) {
		if (filter(i)) result.push_back(i);
	}
	return result;
}
/**
* update meetings
* @param a lambda function as the filter
* @param a lambda function as the method to update the meeting
* @return the number of updated meetings
*/
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
	std::function<void(Meeting &)> switcher) {
	int result = 0;
	auto i = m_meetingList.end();
	for (auto it = m_meetingList.begin(); it != m_meetingList.end(); ++it) {
		if (filter(*it)) {
			switcher(*it);
			if ((*it).getParticipator().empty()) {i = it;}
			++result;
		}
	}
	if (result) m_dirty = true;
	if (i != m_meetingList.end()) {m_meetingList.erase(i);}
	return result;
}

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int result = 0;
	for (auto it = m_meetingList.begin(); it != m_meetingList.end();) {
		if (filter(*it)) {
			it = m_meetingList.erase(it);
			result++;
		}
		else ++it;
	}
	if (result) m_dirty = true;
	return result;
}

/**
* sync with the file
*/
bool Storage::sync(void) {
	bool temp = false;
	if (m_dirty) {
		temp = writeToFile();
		m_dirty = false;
	}
	return temp;
}