#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <regex>

std::map<std::string, std::vector<int>> wordLocations;

// Funkcija, kuri pasalina skyrybos zenklus is zodzio galo
void removePunctuation(std::string& word) {
    word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
}

// Funkcija, kuri konvertuoja zodi i mazasias raides
void toLowerCase(std::string& word) {
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
}

// Funkcija, kuri atspausdina cross-reference tipo lentele ir URL adresus i faila
void printResultsToFile(const std::map<std::string, std::vector<int>>& wordLocations, const std::vector<std::string>& urls, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        // Zodziu lenteles header'is
        outputFile << std::left << std::setw(24) << "| Žodis" << std::setw(15) << "| Pasikartojimai " << std::setw(15) << "| Eilutės |" << std::endl;
        outputFile << std::string(56, '-') << std::endl;

        for (const auto& pair : wordLocations) {
            const std::vector<int>& locations = pair.second;

            // Tikriname, ar zodis pasikartoja daugiau nei viena karta ir ar žodis nera skaicius
            if (locations.size() > 1 && !std::all_of(pair.first.begin(), pair.first.end(), ::isdigit)) {
                outputFile << "| " << std::left << std::setw(20) << pair.first << " | " << std::setw(16) << locations.size() << " | ";

                //isveda kiekivena vieta kur tekstas pasikartojo
                for (size_t i = 0; i < locations.size(); ++i) {
                    outputFile << locations[i];
                    if (i < locations.size() - 1) {
                        outputFile << ", ";
                    }
                }

                outputFile << " |" << std::endl;
            }
        }

        // Isveda URL adresus
        outputFile << "\nSurasti URL adresai:\n";
        for (const auto& url : urls) {
            outputFile << url << std::endl;
        }

        outputFile.close();
        std::cout << "Rezultatai isvesti i faila: " << filename << std::endl;
    } else {
        std::cerr << "Nepavyko atidaryti failo " << filename << " rezultatams isvesti." << std::endl;
    }
}
// Funkcija, kuri isgauna URL adresus is teksto
std::vector<std::string> findURLs(const std::string& text) {
    std::vector<std::string> urls;
    std::regex urlPattern(R"(\b(?:https?://|www\.)\S+\b)");

    std::sregex_iterator iter(text.begin(), text.end(), urlPattern);
    std::sregex_iterator end;

    while (iter != end) {
        urls.push_back(iter->str());
        ++iter;
    }

    return urls;
}

int main() {
    std::ifstream inputFile("tekstas.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Nepavyko atidaryti failo." << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 1;
    while (getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string word;

        while (iss >> word) {
            removePunctuation(word);
            toLowerCase(word);

            if (!word.empty() && word.find_first_of(".,-„“–“”") == std::string::npos) {
                wordLocations[word].push_back(lineNumber);
            }
        }

        lineNumber++;
    }

    inputFile.close();

    std::ifstream urlInputFile("tekstas.txt");
    std::string fullText((std::istreambuf_iterator<char>(urlInputFile)), std::istreambuf_iterator<char>());
    urlInputFile.close();

    std::vector<std::string> urls = findURLs(fullText);

    printResultsToFile(wordLocations, urls, "rezultatai.txt");

    return 0;
}
