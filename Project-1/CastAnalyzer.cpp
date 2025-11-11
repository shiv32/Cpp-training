#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>
#include <map>
#include <iomanip>

namespace fs = std::filesystem;

struct CastOccurrence
{
    std::string castType;
    std::string line;
    size_t lineNumber;
    std::string context; // Stores a few lines before and after the cast
};

struct FileAnalysis
{
    std::vector<CastOccurrence> occurrences;
};

class CastAnalyzer
{
private:
    std::map<std::string, FileAnalysis> fileResults;
    std::vector<std::string> castTypes = {
        "static_cast",
        "dynamic_cast",
        "const_cast",
        "reinterpret_cast"};

    bool isCppFile(const std::string &path)
    {
        std::string ext = fs::path(path).extension().string();
        return ext == ".cpp" || ext == ".h" || ext == ".hpp";
    }

    std::string getContext(const std::vector<std::string> &lines, size_t castLine, size_t contextSize = 2)
    {
        std::string context;
        size_t start = (castLine > contextSize) ? castLine - contextSize : 0;
        size_t end = std::min(lines.size(), castLine + contextSize + 1);

        for (size_t i = start; i < end; ++i)
        {
            context += std::to_string(i + 1) + ": " + lines[i] + "\n";
        }
        return context;
    }

    void analyzeFile(const std::string &filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }

        FileAnalysis analysis;
        for (size_t i = 0; i < lines.size(); ++i)
        {
            for (const auto &castType : castTypes)
            {
                std::regex castPattern(castType + "\\s*<.*?>\\s*\\(");
                if (std::regex_search(lines[i], castPattern))
                {
                    CastOccurrence occurrence;
                    occurrence.castType = castType;
                    occurrence.line = lines[i];
                    occurrence.lineNumber = i + 1;
                    occurrence.context = getContext(lines, i);
                    analysis.occurrences.push_back(occurrence);
                }
            }
        }

        if (!analysis.occurrences.empty())
        {
            fileResults[filepath] = analysis;
        }
    }

public:
    void analyzePath(const std::string &path)
    {
        try
        {
            for (const auto &entry : fs::recursive_directory_iterator(path))
            {
                if (entry.is_regular_file() && isCppFile(entry.path().string()))
                {
                    analyzeFile(entry.path().string());
                }
            }
        }
        catch (const fs::filesystem_error &e)
        {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }
    }

    void displayMenu()
    {
        while (true)
        {
            std::cout << "\n=== Cast Analyzer Menu ===\n";
            std::cout << "1. Show summary of all files\n";
            std::cout << "2. Show detailed analysis for a specific file\n";
            std::cout << "3. Search by cast type\n";
            std::cout << "4. Exit\n";
            std::cout << "Enter your choice (1-4): ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            switch (choice)
            {
            case 1:
                showSummary();
                break;
            case 2:
                showFileDetails();
                break;
            case 3:
                searchByCastType();
                break;
            case 4:
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

private:
    void showSummary()
    {
        std::cout << "\n=== Summary of Cast Usage ===\n";
        for (const auto &[file, analysis] : fileResults)
        {
            std::cout << "\nFile: " << file << "\n";
            std::cout << "Total casts found: " << analysis.occurrences.size() << "\n";

            std::map<std::string, int> castCounts;
            for (const auto &occ : analysis.occurrences)
            {
                castCounts[occ.castType]++;
            }

            for (const auto &[type, count] : castCounts)
            {
                std::cout << "  " << type << ": " << count << "\n";
            }
        }
    }

    void showFileDetails()
    {
        std::cout << "\nAvailable files:\n";
        int index = 1;
        std::vector<std::string> files;
        for (const auto &[file, _] : fileResults)
        {
            std::cout << index++ << ". " << file << "\n";
            files.push_back(file);
        }

        std::cout << "Enter file number: ";
        int fileChoice;
        std::cin >> fileChoice;
        std::cin.ignore();

        if (fileChoice < 1 || fileChoice > files.size())
        {
            std::cout << "Invalid file number.\n";
            return;
        }

        const auto &file = files[fileChoice - 1];
        const auto &analysis = fileResults[file];

        std::cout << "\nDetailed analysis for: " << file << "\n";
        for (const auto &occ : analysis.occurrences)
        {
            std::cout << "\n=== " << occ.castType << " at line " << occ.lineNumber << " ===\n";
            std::cout << "Context:\n"
                      << occ.context << "\n";
        }
    }

    void searchByCastType()
    {
        std::cout << "\nAvailable cast types:\n";
        for (size_t i = 0; i < castTypes.size(); ++i)
        {
            std::cout << i + 1 << ". " << castTypes[i] << "\n";
        }

        std::cout << "Enter cast type number: ";
        int typeChoice;
        std::cin >> typeChoice;
        std::cin.ignore();

        if (typeChoice < 1 || typeChoice > castTypes.size())
        {
            std::cout << "Invalid cast type.\n";
            return;
        }

        const std::string &selectedType = castTypes[typeChoice - 1];
        std::cout << "\nOccurrences of " << selectedType << ":\n";

        for (const auto &[file, analysis] : fileResults)
        {
            for (const auto &occ : analysis.occurrences)
            {
                if (occ.castType == selectedType)
                {
                    std::cout << "\nFile: " << file << "\n";
                    std::cout << "Line " << occ.lineNumber << ":\n";
                    std::cout << occ.context << "\n";
                }
            }
        }
    }
};

int main()
{
    std::cout << "=== C++ Cast Analyzer ===\n";
    std::cout << "Enter the directory path to analyze: ";

    std::string dirPath;
    std::getline(std::cin, dirPath);

    CastAnalyzer analyzer;

    std::cout << "Analyzing files...\n";
    analyzer.analyzePath(dirPath);

    analyzer.displayMenu();

    return 0;
}
