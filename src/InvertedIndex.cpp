#include "InvertedIndex.h"
#include <utility>
#include <iostream>
#include <regex>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    if (docs.empty()) {
        std::cout << "WARNING! The are not \"txt\" files in dataBase." << std::endl;
    } else {
        freq_dictionary.clear();
        for (int i = 0; i < docs.size(); i++) {
            indexWords(docs[i]);
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    } else
        return {};
}

std::size_t InvertedIndex::indexWords(const std::string &source) {
    std::regex someWord("[a-z|A-Z]+");
   // std::match_results firstWord();
    std::cout << std::regex_replace(source, someWord, "[$&]") << '\n';

}