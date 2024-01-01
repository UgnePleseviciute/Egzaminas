#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>

// Funkcija, kuri pašalina skyrybos ženklus iš žodžio galo
void removePunctuation(std::string& word) {
    word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
}

// Funkcija, kuri konvertuoja žodį į mažąsias raides
void toLowerCase(std::string& word) {
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
}

// Funkcija, kuri atspausdina cross-reference tipo lentelę į failą
void printCrossReferenceToFile(const std::map<std::string, std::vector<int>>& wordLocations, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        // Išvedame lentelės antraštę
        outputFile << std::left << std::setw(24) << "| Žodis" << std::setw(10) << "| Pasikartojimai " <<std::setw(5) << "| Eilutės |" << std::endl;
        outputFile << std::string(53, '-') << std::endl;

        for (const auto& pair : wordLocations) {
            const std::vector<int>& locations = pair.second;

            // Tikriname, ar žodas pasikartoja daugiau nei vieną kartą
            if (locations.size() > 1) {
                outputFile << "| " << std::left << std::setw(20) << pair.first << " | " << std::setw(14) << locations.size() << " | ";

                // Išvedame kiekvieną teksto vietą, kurioje žodis pasikartojo
                for (size_t i = 0; i < locations.size(); ++i) {
                    outputFile << locations[i];
                    if (i < locations.size() - 1) {
                        outputFile << ", ";
                    }
                }

                outputFile << " |" << std::endl;
            }
        }

        outputFile.close();
        std::cout << "Cross-reference tipo lentelė isvesta i faila: " << filename << std::endl;
    } else {
        std::cerr << "Nepavyko atidaryti failo " << filename << " rezultatams isvesti." << std::endl;
    }
}

int main() {
    std::ifstream inputFile("tekstas.txt");
    std::map<std::string, std::vector<int>> wordLocations;

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
            // Pašaliname skyrybos ženklus ir konvertuojame į mažąsias raides
            removePunctuation(word);
            toLowerCase(word);

            // Atnaujiname žodžio vietas tekste
            wordLocations[word].push_back(lineNumber);
        }

        lineNumber++;
    }

    inputFile.close();

    // Išvedame cross-reference tipo lentelę į failą
    printCrossReferenceToFile(wordLocations, "rezultatai.txt");

    return 0;
}
