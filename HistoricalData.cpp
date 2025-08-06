#include "HistoricalData.h"

// Constructor
HistoricalData::HistoricalData(){
    // Initialize with no sessions (empty vector)
}

// Add a new scan session to the history
void HistoricalData::addScanSession(const QString& name, const QString& date, const ScanSession& scan) {
    sessions.emplace_back(name, date, scan);
}

// Remove all scan sessions with the specified name
void HistoricalData::removeScanSessions(const QString& name){
    for(auto it = sessions.begin(); it != sessions.end();){
        if(std::get<0>(*it) == name){
            it = sessions.erase(it);
        }else{
            ++it;
        }
    }
}

// Get all scan sessions
std::vector<std::tuple<QString, QString, ScanSession>>& HistoricalData::getAllScanSessions(){
    return sessions;
}
