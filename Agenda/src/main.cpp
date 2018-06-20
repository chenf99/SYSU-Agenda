#include "Date.hpp"
#include "User.hpp"
#include "Meeting.hpp"
#include "Storage.hpp"
#include "AgendaService.hpp"
#include <memory>
#include <iostream>
using namespace std;

int main() {
	AgendaService test;
	test.addMeetingParticipator("maomao", "maomao", "haha");
	std::list<User> user = test.listAllUsers();
	vector<string> userName;
	vector<string> userName2;
	userName2.push_back("ppc");
	for (auto i:user) {
		cout << "user:" << i.getName() << endl;
		if (i.getName() != "cbb" && i.getName() != "ppc") userName.push_back(i.getName());
		list<Meeting> meeting = test.listAllMeetings(i.getName());
		if (meeting.empty()) cout << "meeting: ";
		for (auto j:meeting) cout << j.getTitle() << " ";
		cout << endl;
	}
	//cout << test.userRegister("cbb", "ppc", "fuck", "asd") << endl;
	//cout << test.createMeeting("cbb", "jimao", "2017-06-09/21:36","2017-06-09/21:46", userName) << endl;
	//cout << test.addMeetingParticipator("maomao", "maomao2", "cbb") << endl;
	//cout << test.userRegister("ppc", "cbb", "fucks", "asd") << endl;
	//cout << test.createMeeting("ppc", "jimao3", "2017-06-09/21:46", "2017-06-09/21:50", userName) << endl;
	//cout << test.addMeetingParticipator("cbb", "jimao3", "ppc") << endl;
	
	//cout << test.createMeeting("cbb", "jimao2", "2017-06-09/21:26","2017-06-09/21:36", userName2) << endl;
	//cout << test.deleteUser("ppc", "cbb") << endl;
	return 0;
}