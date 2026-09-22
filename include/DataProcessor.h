#pragma once
#include "DataModels.h"
#include <vector>
#include <map>
#include <string>

namespace App::Processing {
    // Calculates total revenue (quantity * price) for each unique product
    std::map<std::string, double> calculateRevenueByProduct(const std::vector<App::Models::SalesRecord>& records);
}
