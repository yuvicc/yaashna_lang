#include <cctype>
#include <cstddef>
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

std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens {};    
    std::string buf;
    for (size_t i{0uz}; i < str.size(); ++i) {
        char c = str[i];
        if(std::isspace(c)) continue;
        if(std::isalpha(c)) {
            buf.push_back(c);
            i++;
            while(i < str.size() && std::isalnum(str[i])) {
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
            while(i < str.size() && std::isdigit(str[i])) {
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

std::string token_to_asm(const std::vector<Token>& tokens) {
    
    std::stringstream output;
    output << "global _start\n_start:\n";

    for(int i{0}; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        if(token.tokenType == TokenType::_return) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).tokenType == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).tokenType == TokenType::semi) {
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }
    }

    return output.str();
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
        
        if(file.is_open()) {
            std::cerr << "Cannot open file: " << argv[1] << "\n";
            return EXIT_FAILURE;
        }

        sstream << file.rdbuf();
        contents = sstream.str();
    }
    
    std::vector<Token> tokens = tokenize(contents);
    std::string output;

    {
        std::fstream outputfile("./hello.asm", std::ios::out);
        outputfile << token_to_asm(tokens);
    }

    system("nasm -felf64 hello.asm");
    system("ld -o hello hello.o");





    return EXIT_SUCCESS;
}
