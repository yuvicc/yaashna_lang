#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    _return,
    int_lit,
    semi
};

struct Token {
    TokenType tokenType;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(std::string& str) {
    std::vector<Token> tokens {};    
    std::string buf;
    for (int i{0}; i < str.size(); ++i) {
        char c = str[i];
        if(std::isspace(c)) continue;
        if(std::isalpha(c)) {
            buf.push_back(c);
            i++;
            while(std::isalnum(str[i])) {
                buf.push_back(str[i]);

                i++;
            }
            --i;

            if (buf == "return") {
                tokens.push_back({.tokenType = TokenType::_return});
                buf.clear();
            } else {
                std::cerr << "I messed up everything!";
                exit(EXIT_FAILURE);
            }
        } else if (std::isdigit(c)) {
            buf.push_back(c);
            i++;
            while(std::isdigit(str[i])) {
                buf.push_back(str[i]);
                i++;
            }
            i--;
            tokens.push_back({.tokenType = TokenType::int_lit, .value = buf});
            buf.clear();
        } else if (c == ';') {
            tokens.push_back({TokenType::semi});
        } else {
            std::cerr << "You messed up again!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}


int main (int argc, char* argv[]) {
    
    if(argc < 2) {
        std::cerr << "Need path to program!";
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream sstream;
        std::fstream file(argv[1], std::ios::in);
        sstream << file.rdbuf();
        contents = sstream.str();
    }
    
    std::vector<Token> token = tokenize(contents);
    std::cout << "Hello succedded"; 
    return EXIT_SUCCESS;
}
