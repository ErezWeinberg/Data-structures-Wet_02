# Data Structures - Wet Assignment 2

## Course Information
- **Course:** 234218 Data Structures 1
- **Assignment:** Wet Exercise #2

## Project Overview

This project implements a data management system for jockey racing teams using advanced data structures. The system manages teams, jockeys, and their win-loss records, supporting operations like team mergers and record-based team unions.

## Problem Description

The system manages a jockey racing league where:
- **Teams** compete against each other
- **Jockeys** (riders) belong to teams
- Each jockey maintains a personal **record** (wins - losses)
- Each team maintains a collective **record** (sum of all jockeys' records)
- Teams can merge together
- Teams with opposite records (e.g., +5 and -5) can be united

## Data Structures Used

### Core Data Structures
1. **Union-Find (Disjoint Set)** - Manages team mergers with path compression
2. **HashMap** - Custom hash table implementation for O(1) average lookups
3. **Generic Node** - Template-based nodes for union-find structure

### Key Components

#### HashMap (`HashMap.h`)
- Custom hash table with chaining
- Prime number capacity for better distribution
- Dynamic resizing with rehashing
- Supports duplicate key handling for record-based lookups

#### Generic Node (`GenericNode.h`)
- Template-based node for Union-Find structure
- Stores participant data (Team or Jockey)
- Maintains parent pointer and subtree size
- Supports path compression optimization

#### Participant Hierarchy (`Participant.h`)
- Base `Participant` class with ID and record tracking
- `Team` class extending Participant
- `Jockey` class extending Participant

## Main Operations

### 1. add_team(teamId)
Adds a new team with no jockeys.
- **Time Complexity:** O(1) average
- **Returns:** SUCCESS, INVALID_INPUT, FAILURE, or ALLOCATION_ERROR

### 2. add_jockey(jockeyId, teamId)
Adds a jockey to a specific team with initial record of 0.
- **Time Complexity:** O(1) average
- **Returns:** SUCCESS, INVALID_INPUT, FAILURE, or ALLOCATION_ERROR

### 3. update_match(victoriousJockeyId, losingJockeyId)
Updates records after a match between two jockeys from different teams.
- Winner's record increases by 1
- Loser's record decreases by 1
- Both team records are updated
- **Time Complexity:** O(log* m) amortized
- **Returns:** SUCCESS, INVALID_INPUT, FAILURE, or ALLOCATION_ERROR

### 4. merge_teams(teamId1, teamId2)
Merges two teams into one.
- The team with better record keeps its ID
- In case of tie, teamId1 is kept
- Uses union-by-size optimization
- **Time Complexity:** O(log* m) amortized
- **Returns:** SUCCESS, INVALID_INPUT, FAILURE, or ALLOCATION_ERROR

### 5. unite_by_record(record)
Unites exactly 2 teams with records +record and -record.
- Team with positive record keeps its ID
- **Time Complexity:** O(log* m) amortized
- **Returns:** SUCCESS, INVALID_INPUT, FAILURE, or ALLOCATION_ERROR

### 6. get_jockey_record(jockeyId)
Returns the record of a specific jockey.
- **Time Complexity:** O(1) average
- **Returns:** The jockey's record or error status

### 7. get_team_record(teamId)
Returns the collective record of a team.
- **Time Complexity:** O(1) average
- **Returns:** The team's record or error status

## Implementation Details

### Union-Find Optimizations
- **Union by Size:** Smaller trees are attached to larger trees
- **Path Compression:** Paths are flattened during find operations
- Achieves O(log* m) amortized time complexity

### HashMap Details
- Initial capacity: 10007 (prime number)
- Collision resolution: Separate chaining using linked lists
- Load factor threshold triggers rehashing
- Supports multiple values per key for record tracking

## File Structure

```
.
├── plains25a2.h           # Main interface (DO NOT MODIFY)
├── plains25a2.cpp         # Main implementation
├── wet2util.h             # Utility types (DO NOT MODIFY)
├── main.cpp               # Main program (READ ONLY)
├── HashMap.h              # Custom hash table implementation
├── GenericNode.h          # Union-Find node structure
├── Participant.h          # Base classes for Team and Jockey
├── List.h                 # Linked list for hash chaining
├── UnionFind.h/.cpp       # Union-Find data structure
├── team.h/.cpp            # Team class (alternative implementation)
├── jockey.h/.cpp          # Jockey class (alternative implementation)
├── AvlTree.h              # AVL tree (if used)
├── run_tests.py           # Test runner script
├── tests/                 # Test cases directory
│   ├── test10.in/.out
│   ├── test20.in/.out
│   ├── test30.in/.out
│   └── test40.in/.out
├── dont_include.txt       # Prohibited STL headers
└── DS_wet2_Winter_2024-2025.pdf  # Assignment specification
```

## Building and Running

### Compilation
```bash
g++ -std=c++11 -DNDEBUG -Wall -o plains main.cpp plains25a2.cpp UnionFind.cpp team.cpp jockey.cpp
```

### Running Tests
```bash
python3 run_tests.py
```

### Manual Testing
```bash
./plains < tests/test10.in
```

## Input/Output Format

### Input Commands
- `add_team <teamId>`
- `add_jockey <jockeyId> <teamId>`
- `update_match <winnerJockeyId> <loserJockeyId>`
- `merge_teams <teamId1> <teamId2>`
- `unite_by_record <record>`
- `get_jockey_record <jockeyId>`
- `get_team_record <teamId>`

### Output Format
Each command outputs:
```
<command>: <STATUS> [, <result>]
```
Status codes:
- `SUCCESS` - Operation completed successfully
- `ALLOCATION_ERROR` - Memory allocation failed
- `INVALID_INPUT` - Invalid parameters provided
- `FAILURE` - Operation failed (e.g., duplicate ID, non-existent entity)

## Restrictions

### Prohibited STL Components
The following STL containers and headers are **NOT allowed** (see `dont_include.txt`):
- All STL containers (vector, list, map, set, etc.)
- Algorithm header
- String class
- Regex
- Thread-related headers

### Allowed Components
- `<memory>` for smart pointers (shared_ptr, unique_ptr)
- `<iostream>` for I/O
- `<cassert>` for assertions
- Basic C++ features and templates

## Complexity Requirements

| Operation | Time Complexity |
|-----------|----------------|
| add_team | O(1) average |
| add_jockey | O(1) average |
| update_match | O(log* m) amortized |
| merge_teams | O(log* m) amortized |
| unite_by_record | O(log* m) amortized |
| get_jockey_record | O(1) average |
| get_team_record | O(1) average |

Where:
- m = number of teams
- log* = iterated logarithm (inverse Ackermann function)

## Memory Management

- Uses smart pointers (`shared_ptr`) for participant data
- Raw pointers for Union-Find structure to avoid circular references
- Proper cleanup in destructor to prevent memory leaks
- All allocations must be checked and handled appropriately

## Error Handling

All functions return appropriate status codes:
- **SUCCESS (0):** Operation completed successfully
- **ALLOCATION_ERROR (1):** Memory allocation or release problem
- **INVALID_INPUT (2):** Invalid input parameters
- **FAILURE (3):** Operation failed for logical reasons

## Testing

The project includes comprehensive test cases:
- `test10`: Basic functionality tests
- `test20`: Medium-scale tests
- `test30`: Large-scale tests
- `test40`: Stress tests

Each test includes:
- `.in` file: Input commands
- `.out` file: Expected output
- `.res` file: Actual output (generated during testing)

## Academic Integrity

This is a course assignment. Ensure you:
- Write your own code
- Do not share solutions
- Follow course guidelines
- Properly attribute any external resources

## Authors

Erez Weinberg

## License

This is an academic project for Technion's Data Structures course.
