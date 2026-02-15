#include <cstdlib>
#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <cctype>

enum class TokenType {
    _return,
    int_lit,
    semi
};

struct Token {
    TokenType tokenType;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    inline explicit Tokenizer(const std::string contents)
    : m_contents(std::move(contents))
    {}

    inline std::vector<Token> tokenize() {
        
        std::vector<Token> tokens {};    
        std::string buf;

        while(peek().has_value()) {
            if(std::isspace(peek().value())) { consume(); continue; }
            else if (std::isalpha(peek().value())) {
                buf.push_back(consume());
                while(peek().has_value() && std::isalnum(peek().value())) {
                    buf.push_back(consume());     
                }
                if(buf == "return") {
                    tokens.push_back({.tokenType = TokenType::_return});
                    buf.clear();
                } else {
                    std::cerr << "You messed up!";
                    exit(EXIT_FAILURE);
                }
            } else if (std::isdigit(peek().value())) {
                buf.push_back(consume());
                while(peek().has_value() && std::isdigit(peek().value())) {
                    buf.push_back(consume());
                }
                tokens.push_back({.tokenType = TokenType::int_lit, .value = buf});
                buf.clear();
            } else if(peek().value() == ';') {
                consume();
                tokens.push_back({.tokenType = TokenType::semi});
            } else {
                std::cerr << "You messed up brother!";
                exit(EXIT_FAILURE);
            }
        }
        return tokens;
    }

private: 
    std::string m_contents;
    int m_index = 0;

    char consume() {
        return m_contents.at(m_index++);
    }

    [[nodiscard]] std::optional<char> peek(int ahead = 0) const {
        if(m_index + ahead >= m_contents.size()) {
            return {};
        } else {
            return m_contents.at(m_index + ahead);
        }
    }
};
