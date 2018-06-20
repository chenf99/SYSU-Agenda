#include "AgendaService.hpp"
/**
 * constructor
 */
AgendaService::AgendaService() {
	m_storage = nullptr;
	startAgenda();
}

/**
 * destructor
 */
AgendaService::~AgendaService() {
	quitAgenda();
	m_storage = nullptr;
}

/**
 * check if the username match password
 * @param userName the username want to login
 * @param password the password user enter
 * @return if success, true will be returned
 */
bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
	std::list<User> user = m_storage->queryUser(
		[&](const User& u)-> bool{return (u.getName() == userName);});
	if (!user.empty()) {
		if (user.front().getPassword() == password) return true;
		return false;
	}
	return false;
}

/**
 * regist a user
 * @param userName new user's username
 * @param password new user's password
 * @param email new user's email
 * @param phone new user's phone
 * @return if success, true will be returned
 */
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
const std::string &email, const std::string &phone) {
	std::list<User> user = m_storage->queryUser(
		[&](const User& u)-> bool{return (u.getName() == userName);});
	if (user.empty()) {
		m_storage->createUser(User(userName, password, email, phone));
		return true;
	}
	return false;
}

/**
 * delete a user
 * @param userName user's username
 * @param password user's password
 * @return if success, true will be returned
 */
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
	if (m_storage->deleteUser([&](const User& u)-> bool{
		if (u.getName() == userName) {
			if (u.getPassword() == password) return true;
			return false;
		}
		return false;
	}) == 1) {
		deleteAllMeetings(userName);
		std::list<Meeting> meeting = listAllParticipateMeetings(userName);
		for (auto i:meeting) {
			removeMeetingParticipator(i.getSponsor(), i.getTitle(), userName);
		}
		return true;
	}
	return false;
}

/**
 * list all users from storage
 * @return a user list result
 */
std::list<User> AgendaService::listAllUsers(void) const{
	std::list<User> user = m_storage->queryUser([](const User&)-> bool{return true;});
	return user;
}

/**
 * create a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @param startData the meeting's start date
 * @param endData the meeting's end date
 * @return if success, true will be returned
 */
bool AgendaService::createMeeting(const std::string &userName, const std::string &title, 
const std::string &startDate, const std::string &endDate, const std::vector<std::string> &participator) {
	Date start = Date::stringToDate(startDate), end = Date::stringToDate(endDate);
	if (!Date::isValid(start) || !Date::isValid(end)) return false;
	if (start >= end) return false;

	for (auto i:participator) {
		if (i == userName) return false;
	}
	for (auto i = participator.begin(); i != participator.end(); ++i) {
		for (auto j = participator.begin(); j != participator.end(); ++j) {
			if (i != j && *i == *j) return false;
		}
	}
	std::list<User> sponsor = m_storage->queryUser(
		[&](const User& u)-> bool{if (u.getName() == userName) return true;return false;} );
	if (sponsor.empty()) return false;
	std::list<User> parti = m_storage->queryUser(
		[&](const User& u)-> bool{
			for (auto i:participator) {
				if (i == u.getName()) return true;
			}
			return false;
		});
	if (parti.size() != participator.size()) return false;

	std::list<Meeting> meeting = m_storage->queryMeeting(
		[&](const Meeting& m)-> bool{if (m.getTitle() == title) return true;return false;} );
	if (!meeting.empty()) return false;

	std::list<Meeting> isInMeeting = meetingQuery(userName, startDate, endDate);
	if (isInMeeting.size() == 0 || isInMeeting.size() == 1 && 
	(start == isInMeeting.front().getEndDate() || end == isInMeeting.front().getStartDate())) {
		for (auto i:participator) {
			isInMeeting = meetingQuery(i, startDate, endDate);
			if (isInMeeting.size() == 0 || isInMeeting.size() == 1 && 
			(start == isInMeeting.front().getEndDate() || end == isInMeeting.front().getStartDate())) continue;
			else return false;
		}
		m_storage->createMeeting(Meeting(userName, participator, start, end, title));
		return true;
	}
	return false;
}

/**
 * add a participator to a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::addMeetingParticipator(const std::string &userName, const std::string &title,
const std::string &participator) {
	std::list<User> user = m_storage->queryUser([&](const User& u)-> bool{
		if (u.getName() == participator) return true;
		return false;
	});
	if (user.empty()) return false;

	if (m_storage->updateMeeting([&](const Meeting& m)-> bool{
		if (m.getSponsor() == userName && m.getTitle() == title) {
			if (m.isParticipator(participator)) return false;
			std::list<Meeting> isInMeeting = meetingQuery(participator, 
				Date::dateToString(m.getStartDate()), Date::dateToString(m.getEndDate()));
			if (isInMeeting.size() == 0 || isInMeeting.size() == 1 && 
				(m.getStartDate() == isInMeeting.front().getEndDate() || 
				m.getEndDate() == isInMeeting.front().getStartDate())) return true;
			return false;
		}
		return false;
	}, [&](Meeting& m)-> void{
		m.addParticipator(participator);
	}) == 1) return true;
	return false;
}

/**
 * remove a participator from a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::removeMeetingParticipator(const std::string &userName, const std::string &title,
 const std::string &participator) {
	if (m_storage->updateMeeting([&](const Meeting& m)-> bool{
		if (m.getTitle() == title && m.isParticipator(participator) && m.getSponsor() == userName) return true;
		return false;
	}, [&](Meeting& m)-> void{
		m.removeParticipator(participator);
	})) return true;
	return false;
}

/**
 * quit from a meeting
 * @param userName the current userName. no need to be the sponsor
 * @param title the meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
	if (m_storage->updateMeeting([&](const Meeting& m)-> bool{
		if (m.getTitle() == title && m.isParticipator(userName)) return true;
		return false;
	}, [&](Meeting& m)-> void{
		m.removeParticipator(userName);
	})) return true;
	return false;
}

/**
 * search a meeting by username and title
 * @param uesrName the sponsor's userName
 * @param title the meeting's title
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &title) const{
	return m_storage->queryMeeting([&](const Meeting& m)-> bool{
		if (m.getSponsor() == userName && m.getTitle() == title) return true;
		return false;
	});
}
/**
 * search a meeting by username, time interval
 * @param uesrName the sponsor's userName
 * @param startDate time interval's start date
 * @param endDate time interval's end date
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &startDate,
	const std::string &endDate) const{
	std::list<Meeting> emptylist;
	Date start = Date::stringToDate(startDate), end = Date::stringToDate(endDate);
	if (!Date::isValid(start) || !Date::isValid(end)) return emptylist;
	if (start > end) return emptylist;
	return m_storage->queryMeeting([&](const Meeting& m)-> bool{
		if (m.getSponsor() == userName || m.isParticipator(userName)) {
			if (m.getEndDate() < start || m.getStartDate() > end) return false;
			return true;
		}
		return false;
	});
}

/**
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const{
	return m_storage->queryMeeting([&](const Meeting& m)-> bool{
		if (m.getSponsor() == userName || m.isParticipator(userName)) return true;
		return false;
	});
}

/**
 * list all meetings the user sponsor
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
	return m_storage->queryMeeting([&](const Meeting& m)-> bool{
		if (m.getSponsor() == userName) return true;
		return false;
	});
}

/**
 * list all meetings the user take part in and sponsor by other
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const{
	return m_storage->queryMeeting([&](const Meeting& m)-> bool{
		if (m.isParticipator(userName)) return true;
		return false;
	});
}

/**
 * delete a meeting by title and its sponsor
 * @param userName sponsor's username
 * @param title meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
	if (m_storage->deleteMeeting([&](const Meeting& m)-> bool{
		if (m.getSponsor() == userName && m.getTitle() == title) return true;
		return false;
	}) ) return true;
	return false;
}

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const std::string &userName) {
	if (m_storage->deleteMeeting([&](const Meeting& m)-> bool{
		if (m.getSponsor() == userName) return true;
		return false;
	}) ) return true;
	return false;
}

/**
 * start Agenda service and connect to storage
 */
void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

/**
 * quit Agenda service
 */
void AgendaService::quitAgenda(void) {
	m_storage = nullptr;
}