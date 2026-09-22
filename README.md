# Data-Processing-Application-C++-Project-
# DataProcessingApp

A C++ capstone project built for the **Microsoft C++ course on Coursera**, developed with the assistance of generative AI tools, with a deliberate focus on program logic and clean architecture. The application reads sales data from a CSV file, parses it into strongly-typed records, computes revenue totals per product, and writes a summary back out to CSV.

## Overview

`DataProcessingApp` follows a pipeline architecture: raw text in, structured data through a series of transformations, formatted text out. Each stage has a single responsibility and lives behind its own header/namespace, which keeps parsing, business logic, and file I/O independent of one another — a pattern common in production-grade C++ codebases.

**Pipeline:**

<img width="1408" height="768" alt="image" src="https://github.com/user-attachments/assets/9e9c3b6f-2f66-4a94-a7da-3c75f37b17dd" />

```
data/input.csv → read lines → parse into records → calculate revenue → format output → data/output.csv
```

## Architecture

The application is organized into four responsibilities, each mapped to a namespace and a header/source pair:

| Namespace | Header | Source | Responsibility |
|---|---|---|---|
| `App::Models` | `DataModels.h` | — | Defines the `SalesRecord` data structure |
| `App::IO` | `FileIO.h` | `FileIO.cpp` | Reading/writing raw lines from/to disk |
| `App::Parsing` | `DataParser.h` | `DataParser.cpp` | Converting raw strings ↔ structured data |
| `App::Processing` | `DataProcessor.h` | `DataProcessor.cpp` | Business logic (revenue calculation) |

`main.cpp` wires these stages together in sequence.

### Data flow, step by step

**1. Read raw strings from `data/input.csv`**
`App::IO::readLines(inputFile)` opens the file stream using RAII (the stream is automatically closed when it goes out of scope, even on early return or exception) and reads it line by line.
→ Output: `std::vector<std::string> rawLines`

**2. Parse strings into data structures**
`App::Parsing::parseCSVLine(line)` iterates over `rawLines`, tokenizing each one on commas, converting fields to their proper types (`std::stoi`, `std::stod`), and catching malformed rows gracefully rather than crashing the program.

```cpp
struct SalesRecord {
    int id;
    std::string productName;
    int quantity;
    double price;
};
```
→ Output: `std::vector<App::Models::SalesRecord> records`

**3. Calculate totals and business logic**
`App::Processing::calculateRevenueByProduct(records)` computes `quantity * price` for each record and accumulates the results into a map keyed by product name.
→ Output: `std::map<std::string, double> revenueSummary`

**4. Prepare results for output**
`App::Parsing::formatOutputLine(product, revenue)` unpacks each `[product, revenue]` pair from `revenueSummary` (via structured bindings) and assembles it into a formatted output line.
→ Output: `std::vector<std::string> outputLines`

**5. Write final strings to `data/output.csv`**
`App::IO::writeLines(outputFile, outputLines)` writes the formatted lines to a new CSV file, saving the summary to disk.

## Project Structure

```
DataProcessingApp/
├── CMakeLists.txt
├── data/
│   ├── input.csv        # Raw sales data
│   └── output.csv        # Generated revenue summary
├── include/
│   ├── DataModels.h       # SalesRecord struct
│   ├── FileIO.h           # File read/write declarations
│   ├── DataParser.h       # CSV parsing / formatting declarations
│   └── DataProcessor.h    # Revenue calculation declarations
└── src/
    ├── main.cpp            # Orchestrates the pipeline
    ├── FileIO.cpp
    ├── DataParser.cpp
    └── DataProcessor.cpp
```

## Prerequisites

- A C++ compiler supporting C++17 or later (for structured bindings — `auto& [product, revenue]`)
- [CMake](https://cmake.org/) (version 3.10+ recommended)
- Standard library only — no external parsing library is required for CSV handling

## Build Instructions

```bash
# Clone the repository
git clone <your-repo-url>
cd DataProcessingApp

# Create a build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .
```

## Usage

```bash
./DataProcessingApp
```

The application reads from `data/input.csv` and writes the revenue summary to `data/output.csv`. *Update this if your `main.cpp` accepts command-line arguments for custom input/output paths.*

## Input Format (`data/input.csv`)

Each row represents one sales transaction:

| Column | Type | Description |
|---|---|---|
| id | int | Record identifier |
| productName | string | Name of the product |
| quantity | int | Units sold |
| price | double | Price per unit |

## Output Format (`data/output.csv`)

Each row represents one product's total revenue:

| Column | Description |
|---|---|
| productName | Name of the product |
| revenue | Sum of `quantity * price` across all matching records |

## Design Notes

- **RAII file handling**: file streams are managed automatically, preventing resource leaks even if an error occurs mid-read.
- **Graceful error handling**: malformed CSV rows are caught during parsing rather than crashing the program.
- **Separation of concerns**: I/O, parsing, and business logic are isolated in their own namespaces, making the codebase easier to test, extend (e.g., adding a JSON parser alongside the CSV one), and maintain.
- **`std::map` for aggregation**: revenue is aggregated per product using a sorted associative container, which naturally keeps output ordered by product name.

## Development Notes

This project was developed with the assistance of generative AI tools as part of the course requirements, with the design and core logic driven by the author.
