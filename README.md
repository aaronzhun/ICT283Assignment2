# ICT283Assignment2
ICT283Assignment2

C++11 console program that loads weather readings from multiple CSV files.

Records are grouped by day using std::map, with each day containing a custom
template BST. BST traversal uses a function-pointer callback to collect records
into the Vector reused from Assignment 1.

Implemented menu options:
1. Monthly wind mean and sample standard deviation
2. Monthly temperature means and sample standard deviations for a selected year
3. Speed, temperature and solar Pearson correlations for a selected month across all loaded years
4. Yearly CSV report with MAD (WIP)
5. Exit

The loader supports reordered columns and rejects duplicate timestamps.
January and February correlation results were checked against Excel PEARSON.

Open Lab11.cbp in CodeBlocks and run from the project folder.
