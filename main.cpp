#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

// Funkcija, kuri skaičiuoja žodzius tekste
std::map<std::string, int> countWords(const std::string &text) {
    std::map<std::string, int> wordCount;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {
        // pasalina skyrybos zenklus nuo zodziu
        while (!word.empty() && !isalnum(word.back())) {
            word.pop_back();
        }

//zodis mazosiomis raidemmis
        for (char &c : word) {
            c = tolower(c);
        }
    //skaiciuoja zodzius
        wordCount[word]++;
    }

    return wordCount;
}

int main() {
    // Nuskaitome tekstą is failo
    std::ifstream inputFile("tekstas.txt");
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string text = buffer.str();

    // skaiciuoja žodžius
    std::map<std::string, int> wordCount = countWords(text);

    // Atidarome faila rezultatams irasyti
    std::ofstream outputFile("rezultatai.txt");

    // isvedame zodzius, kurie pasikartojo daugiau negu 1 karta
    for (const auto &pair : wordCount) {
        if (pair.second > 1) {
            outputFile << pair.first << ": " << pair.second << " kartai" << std::endl;
        }
    }

    return 0;
}
