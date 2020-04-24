/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                    const string& word_list_fname)
{
    vector<std::tuple<std::string, std::string, std::string>> ret;
	tuple<string, string, string> tup;
	string word;
	ifstream words(word_list_fname);
	if (words.is_open()) {
		while (getline(words, word)) {
			if (word.length()>=2) {
				string subs1 = word.substr(1);
				string subs2 = word.substr(0, 1) + word.substr(2);
				if (d.homophones(subs1, subs2) == 1)
					ret.push_back(make_tuple(word, subs1, subs2));
			}
		}
	}
	return ret.empty() ? vector<tuple<string, string, string>>() : ret;
}
