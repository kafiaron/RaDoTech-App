#ifndef HISTORICALDATA_H
#define HISTORICALDATA_H

#include "ScanSession.h"
#include <tuple>
#include <vector>

class HistoricalData {
private:
    // Vector to store scan sessions
    std::vector<std::tuple<QString, QString, ScanSession>> sessions;

public:
    // Constructor
    HistoricalData();

    // Add a new scan session to the history
    void addScanSession(const QString&, const QString&, const ScanSession& scan);

    // Remove all scan sessions with the specified name
    void removeScanSessions(const QString& name);

    // Get all scan sessions
    std::vector<std::tuple<QString, QString, ScanSession>>& getAllScanSessions();
};

#endif // HISTORICALDATA_H
