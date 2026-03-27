#include "Table.h"
#include "Transaction.h"

#include <fstream>

Table::Table(int TID, int LogID): tableID(TID), logID(LogID)
{
    if (name.empty()) {
        fileName = "data/table_" + std::to_string(TID) + ".dat";
    } else {
        fileName = "data/" + name + std::to_string(TID) + ".dat";
    }
    deserializeAccDebts();
}

Table::~Table()
{
    serializeAccDebts();
}

void Table::serializeAccDebts() const
{
    std::ofstream out(fileName, std::ios::binary);
    if (!out.is_open()) {
        return;
    }

    int size = static_cast<int>(accDebts.size());
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (const auto& entry : accDebts) {
        const int accID = entry.first;
        const double debt = entry.second;
        out.write(reinterpret_cast<const char*>(&accID), sizeof(accID));
        out.write(reinterpret_cast<const char*>(&debt), sizeof(debt));
    }
}

void Table::deserializeAccDebts()
{
    std::ifstream in(fileName, std::ios::binary);
    if (!in.is_open()) {
        return;
    }

    accDebts.clear();

    int size = 0;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!in) {
        accDebts.clear();
        return;
    }
    if (size < 0) {
        accDebts.clear();
        return;
    }

    for (int i = 0; i < size; ++i) {
        int accID = 0;
        double debt = 0.0;

        in.read(reinterpret_cast<char*>(&accID), sizeof(accID));
        in.read(reinterpret_cast<char*>(&debt), sizeof(debt));
        if (!in) {
            accDebts.clear();
            return;
        }

        accDebts[accID] = debt;
    }
}

double Table::getAccDebt(int accID) const
{
    try {
        return accDebts.at(accID);
    }
    catch (const std::out_of_range &) {
        throw AccountNotFound();
    }
}

bool Table::hasAccount(int accID) const
{
    return accDebts.find(accID) != accDebts.end();
}

void Table::addAccount(int accID, double initialDebt)
{
    auto result = accDebts.emplace(accID, initialDebt);
    if (!result.second) {
        throw AccountAlreadyExists();
    }
}

void Table::processTransaction(const Transaction &transaction)
{
    if (accDebts.find(transaction.getAccID()) == accDebts.end())
        throw AccountNotFound();
    accDebts[transaction.getAccID()] += transaction.getDebtIncAmount();
}
