#include "AgendaUI.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;

AgendaUI::AgendaUI() {
	m_userName = "";
	m_userPassword = "";
	cout << "Action :\n";
	cout << "l 				- log in by username and password\n";
	cout << "r 				- register user\n";
	cout << "q 				- quit Agenda\n" << endl;
}

void AgendaUI::OperationLoop(void) {
	if (m_userName == "") cout << "Agenda : ~$ ";
	executeOperation(getOperation());
}
/**
 * constructor
 */
void AgendaUI::startAgenda(void) {
	m_agendaService.startAgenda();
}

/**
 * catch user's operation
 * @return the operation
 */
std::string AgendaUI::getOperation() {
	string operation;
	cin >> operation;
	return operation;
}

/**
 * execute the operation
 * @return if the operationloop continue
 */
bool AgendaUI::executeOperation(const std::string &t_operation) {
	
}

/**
 * user Login
 */
void AgendaUI::userLogIn(void) {
	m_agendaService.userLogIn()
}

/**
 * user regist
 */
void AgendaUI::userRegister(void);

/**
 * user logout
 */
void AgendaUI::userLogOut(void);

/**
 * quit the Agenda
 */
void AgendaUI::quitAgenda(void);

/**
 * delete a user from storage
 */
void AgendaUI::deleteUser(void);

/**
 * list all users from storage
 */
void AgendaUI::listAllUsers(void);

/**
 * user create a meeting with someone else
 */
void AgendaUI::createMeeting(void);

/**
 * sponsor add a participator to the meeting
 */
void AgendaUI::addMeetingParticipator(void);

/**
 * sponsor add a participator to the meeting
 */
void AgendaUI::removeMeetingParticipator(void);

/**
 * user quit from meeting
 */
void AgendaUI::quitMeeting(void);

/**
 * list all meetings from storage
 */
void AgendaUI::listAllMeetings(void);

/**
 * list all meetings that this user sponsored
 */
void AgendaUI::listAllSponsorMeetings(void);

/**
 * list all meetings that this user take part in
 */
void AgendaUI::listAllParticipateMeetings(void);

/**
 * search meetings by title from storage
 */
void AgendaUI::queryMeetingByTitle(void);

/**
 * search meetings by timeinterval from storage
 */
void AgendaUI::queryMeetingByTimeInterval(void);

/**
 * delete meetings by title from storage
 */
void AgendaUI::deleteMeetingByTitle(void);

/**
 * delete all meetings that this user sponsored
 */
void AgendaUI::deleteAllMeetings(void);

/**
 * show the meetings in the screen
 */
void AgendaUI::printMeetings(const std::list<Meeting> &t_meetings);