// Implementation of the missing methods for the VotingSystem class

// Get the list of running elections
std::vector<std::string> VotingSystem::getRunningElectionIds() {
    return electionManager.getRunningElectionIds();
}

// Get the list of all elections
std::vector<std::string> VotingSystem::getAllElectionIds() {
    return electionManager.getAllElectionIds();
}

// Display detailed information about an election
void VotingSystem::displayElectionSummary(const std::string& electionId) {
    Election* election = electionManager.getElectionById(electionId);
    if (election != nullptr) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Election ID: " << electionId << std::endl;
        std::cout << "Name: " << election->getName() << std::endl;
        std::cout << "Type: " << election->getType() << std::endl;
        std::cout << "Start Date: " << election->getStartDate() << std::endl;
        std::cout << "End Date: " << election->getEndDate() << std::endl;
        std::cout << "Status: " << (election->isRunning() ? "Running" : "Not Running") << std::endl;
        
        if (election->getType() == "Local") {
            std::cout << "Region: " << static_cast<LocalElection*>(election)->getRegion() << std::endl;
        } else if (election->getType() == "Regional") {
            std::cout << "Region: " << static_cast<RegionalElection*>(election)->getRegion() << std::endl;
        }
        
        std::cout << "Candidates:" << std::endl;
        std::vector<Candidate> candidates = election->getCandidates();
        for (size_t i = 0; i < candidates.size(); i++) {
            std::cout << "  " << i+1 << ". " << candidates[i].getName() << " (" << candidates[i].getPartyAffiliation() << ")" << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
    } else {
        std::cout << "Election not found." << std::endl;
    }
}

// Get the indices of candidates in an election
std::vector<int> VotingSystem::getCandidateIndices(const std::string& electionId) {
    Election* election = electionManager.getElectionById(electionId);
    std::vector<int> indices;
    
    if (election != nullptr) {
        std::vector<Candidate> candidates = election->getCandidates();
        for (size_t i = 0; i < candidates.size(); i++) {
            indices.push_back(static_cast<int>(i));
        }
    }
    
    return indices;
}

// Display detailed information about a candidate
void VotingSystem::displayCandidateSummary(const std::string& electionId, int candidateIndex) {
    Election* election = electionManager.getElectionById(electionId);
    
    if (election != nullptr) {
        std::vector<Candidate> candidates = election->getCandidates();
        
        if (candidateIndex >= 0 && candidateIndex < static_cast<int>(candidates.size())) {
            Candidate candidate = candidates[candidateIndex];
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "Candidate Name: " << candidate.getName() << std::endl;
            std::cout << "Party Affiliation: " << candidate.getPartyAffiliation() << std::endl;
            std::cout << "Votes Received: " << candidate.getVoteCount() << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        } else {
            std::cout << "Invalid candidate index." << std::endl;
        }
    } else {
        std::cout << "Election not found." << std::endl;
    }
}
