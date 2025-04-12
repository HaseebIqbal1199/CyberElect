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
    cout << "3. Running Elections Status" << endl;
    cout << "4. Exit" << endl;
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
                string electionId, candidateId;
                cout << "Enter Election ID: ";
                getline(cin, electionId);
                
                if (votingSystem.hasVoted(electionId)) {
                    cout << "You have already voted in this election." << endl;
                    break;
                }
                
                cout << "Enter Candidate ID: ";
                getline(cin, candidateId);
                
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
                string electionId;
                cout << "Enter Election ID: ";
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

int main() {
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
                    voterMenu();
                } else {
                    cout << "Login failed. Please check your credentials." << endl;
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
                    adminMenu();
                } else {
                    cout << "Login failed. Please check your credentials." << endl;
                }
                break;
            }
            case 3:
                votingSystem.displayRunningElections();
                break;
            case 4:
                cout << "Thank you for using the Online Voting System!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    
    return 0;
}
