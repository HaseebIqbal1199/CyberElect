#include "bin/lib/header/VotingSystem.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

VotingSystem votingSystem;

void displayMainMenu() {
    cout << "\nOnline Voting System" << endl;
    cout << "===================" << endl;
    cout << "1. Login as Voter" << endl;
    cout << "2. Login as Administrator" << endl;
    cout << "3. Register" << endl;
    cout << "4. Running Elections Status" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

void voterMenu() {
    while (true) {
        cout << "\nVoter Menu" << endl;
        cout << "==========" << endl;
        cout << "1. View Active Elections" << endl;
        cout << "2. Cast Vote" << endl;
        cout << "3. Logout" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                votingSystem.viewElections();
                break;
            case 2: {
                // First show active elections
                cout << "\nAvailable Elections:" << endl;
                cout << "===================" << endl;
                votingSystem.displayRunningElections();
                
                string electionId;
                cout << "\nEnter Election ID (or 'back' to return to menu): ";
                getline(cin, electionId);
                
                if (electionId == "back") {
                    break;
                }
                
                if (votingSystem.hasVoted(electionId)) {
                    cout << "You have already voted in this election." << endl;
                    break;
                }
                
                // Show election details again with candidate list
                cout << "\nElection Details:" << endl;
                cout << "================" << endl;
                votingSystem.displayElectionInfo(electionId);
                
                string candidateId;
                cout << "\nEnter Candidate ID (or 'back' to return to menu): ";
                getline(cin, candidateId);
                
                if (candidateId == "back") {
                    break;
                }
                
                if (votingSystem.castVote(electionId, candidateId)) {
                    cout << "Vote cast successfully!" << endl;
                } else {
                    cout << "Failed to cast vote. Please check the election and candidate IDs." << endl;
                }
                break;
            }
            case 3:
                votingSystem.logout();
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}

void adminMenu() {
    while (true) {
        cout << "\nAdministrator Menu" << endl;
        cout << "=================" << endl;
        cout << "1. Create Election" << endl;
        cout << "2. Start Election" << endl;
        cout << "3. End Election" << endl;
        cout << "4. Add Candidate" << endl;
        cout << "5. View Results" << endl;
        cout << "6. View All Elections" << endl;
        cout << "7. Logout" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string name, type, start, end, region;
                int regions = 0;
                
                cout << "Enter election type (local/national/regional): ";
                getline(cin, type);
                cout << "Enter election name: ";
                getline(cin, name);
                cout << "Enter start date: ";
                getline(cin, start);
                cout << "Enter end date: ";
                getline(cin, end);
                
                if (type == "national") {
                    cout << "Enter country: ";
                    getline(cin, region);
                    cout << "Enter number of regions: ";
                    cin >> regions;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else if (type == "regional") {
                    cout << "Enter region: ";
                    getline(cin, region);
                }
                else if (type == "local") {
                    cout << "Enter locality: ";
                    getline(cin, region);
                }
                
                if (votingSystem.createElection(type, name, start, end, region, regions)) {
                    cout << "Election created successfully!" << endl;
                } else {
                    cout << "Failed to create election." << endl;
                }
                break;
            }
            case 2: {
                string electionId;
                cout << "Enter Election ID to start: ";
                getline(cin, electionId);
                if (votingSystem.startElection(electionId)) {
                    cout << "Election started successfully!" << endl;
                } else {
                    cout << "Failed to start election." << endl;
                }
                break;
            }
            case 3: {
                string electionId;
                cout << "Enter Election ID to end: ";
                getline(cin, electionId);
                if (votingSystem.endElection(electionId)) {
                    cout << "Election ended successfully!" << endl;
                } else {
                    cout << "Failed to end election." << endl;
                }
                break;
            }
            case 4: {
                string electionId, name, party;
                cout << "Enter Election ID: ";
                getline(cin, electionId);
                cout << "Enter Candidate Name: ";
                getline(cin, name);
                cout << "Enter Party Affiliation: ";
                getline(cin, party);
                
                if (votingSystem.addCandidate(electionId, name, party)) {
                    cout << "Candidate added successfully!" << endl;
                } else {
                    cout << "Failed to add candidate." << endl;
                }
                break;
            }
            case 5: {
                cout << "\nAvailable Elections:" << endl;
                cout << "-------------------" << endl;
                votingSystem.viewAllElections();
                
                string electionId;
                cout << "\nEnter Election ID: ";
                getline(cin, electionId);
                votingSystem.viewResults(electionId);
                break;
            }
            case 6:
                votingSystem.viewAllElections();
                break;
            case 7:
                votingSystem.logout();
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}

void registrationMenu() {
    while (true) {
        cout << "\nRegistration Menu" << endl;
        cout << "=================" << endl;
        cout << "1. Register as Voter" << endl;
        cout << "2. Register as Administrator" << endl;
        cout << "3. Back to Main Menu" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string username, password, name;
        bool success = false;

        switch (choice) {
            case 1:
                cout << "\nVoter Registration" << endl;
                cout << "=================" << endl;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                cout << "Enter full name: ";
                getline(cin, name);

                success = votingSystem.registerVoter(username, password, name);
                if (success) {
                    cout << "Voter registered successfully!" << endl;
                } else {
                    cout << "Failed to register voter." << endl;
                }
                break;

            case 2:
                cout << "\nAdministrator Registration" << endl;
                cout << "========================" << endl;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                cout << "Enter full name: ";
                getline(cin, name);

                success = votingSystem.registerAdmin(username, password, name);
                if (success) {
                    cout << "Administrator registered successfully!" << endl;
                } else {
                    cout << "Failed to register administrator." << endl;
                }
                break;

            case 3:
                return;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}

int main() {
    // Create necessary directories
    system("mkdir data 2>nul");
    system("mkdir data\\elections 2>nul");
    system("mkdir data\\voters 2>nul");
    system("mkdir data\\admins 2>nul");

    while (true) {
        displayMainMenu();
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string username, password;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                
                if (votingSystem.loginUser(username, password, "voter")) {
                    cout << "Login successful!" << endl;
                    voterMenu();
                } else {
                    cout << "Invalid credentials!" << endl;
                }
                break;
            }
            case 2: {
                string username, password;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                
                if (votingSystem.loginUser(username, password, "admin")) {
                    cout << "Login successful!" << endl;
                    adminMenu();
                } else {
                    cout << "Invalid credentials!" << endl;
                }
                break;
            }
            case 3:
                registrationMenu();
                break;
            case 4:
                votingSystem.displayRunningElections();
                break;
            case 5:
                cout << "Thank you for using the Online Voting System!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    
    return 0;
}
