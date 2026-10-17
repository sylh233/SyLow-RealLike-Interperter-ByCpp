#include <iostream>
#include <scanner.hpp>

namespace cbycpp {
namespace scanner {
void Hello() { std::cout << "Hello" << std::endl; }

Token::Token(int line, TokenTypes type, std::string lexeme)
    : lines(line), type(type), lexeme(lexeme) {}

std::string Token::toString() {
	// std::string literal_str = " ";
	// if (this->literal)
	// 	literal_str += this->literal.value().to_string() + " ";
	return "[" + this->getTypeString() + " \"" + this->lexeme + "\"" +
	       " line:" + std::to_string(this->lines) + "]";
}

std::string Token::getTypeString() { return type_map.at(this->type); }

std::unique_ptr<Scanner::vTokens> Scanner::scan(std::string &b) {
	vTokens *tokens = new vTokens;
	bytes = &b;
	lines = 0, start = 0, current = 0;
	while (!is_end()) {
		start = current;
		char c = cur_advance();
		try {
			switch (c) {
			case '+':
				tokens->emplace_back(lines, PLUS, std::string(1, c));
				break;
			case '-':
				tokens->emplace_back(lines, MINUS, std::string(1, c));
				break;
			case '*':
				tokens->emplace_back(lines, STAR, std::string(1, c));
				break;
			case '/':
				tokens->emplace_back(lines, SLASH, std::string(1, c));
				break;
			case '(':
				tokens->emplace_back(lines, LEFT_PAREN, std::string(1, c));
				break;
			case ')':
				tokens->emplace_back(lines, RIGHT_PAREN, std::string(1, c));
				break;
			case '<':
				if (match('=')) {
					tokens->emplace_back(lines, LESS_EQUAL, "<=");
				} else {
					tokens->emplace_back(lines, LESS, "<");
				}
				break;
			case '>':
				if (match('=')) {
					tokens->emplace_back(lines, GREATER_EQUAL, ">=");
				} else {
					tokens->emplace_back(lines, GREATER, ">");
				}
				break;
			case '=':
				if (match('=')) {
					tokens->emplace_back(lines, EQUAL_EQUAL, "==");
				} else {
					tokens->emplace_back(lines, EQUAL, "=");
				}
				break;
			case '!':
				if (match('=')) {
					tokens->emplace_back(lines, BANG_EQUAL, "!=");
				} else {
					tokens->emplace_back(lines, BANG, "!");
				}
				break;
			case '"':
				scan_string(tokens);
				break;
			case '\'':
				scan_char(tokens);
				break;
			case '\n':
				lines++;
			case '\t':
			case ' ':
				break;
			default:
				if (is_number(c)) {
					scan_number(tokens);
					break;
				} else if (is_letterN_(c)) {
					scan_identifier(tokens);
				} else {
					throw exception::ScanException(c, lines, "Unexpected Char");
				}
				break;
			}
		} catch (exception::ScanException se) {
			std::cout << se.ExceptionMsg() << std::endl;
			// while (!is_end()) {
			// 	current++;
			// }
		}
	}
	return std::unique_ptr<vTokens>(tokens);
}

void Scanner::scan_number(vTokens *tokens) {
	while (!is_end() && is_number(cur_char())) {
		current++;
	}
	if (!is_end() && match('.')) {
		while (!is_end() && is_number(cur_char())) {
			current++;
		}
		tokens->emplace_back(lines, FLOAT,
		                     bytes->substr(start, current - start));
		return;
	}
	// std::string lexeme = bytes->substr(start, current - start);
	// std::uint32_t value = std::stoi(lexeme);
	// std::optional<Variant> variant = Variant(&value, Variant::Type::I32);
	tokens->emplace_back(lines, INT, bytes->substr(start, current - start));
	return;
}

void Scanner::scan_string(std::vector<Token> *tokens) {
	std::string str{"\""};
	bool has_dquote = false;
	while (!is_end()) {
		char c = esc_char();
		str += c;
		if (c == '"') {
			has_dquote = true;
			break;
		}
	}
	if (!has_dquote)
		throw exception::ScanException('\0', lines,
		                               "Expect ' \" ' after String");
	tokens->emplace_back(lines, STRING, str);
	return;
}

void Scanner::scan_char(std::vector<Token> *tokens) {
	if (!is_end()) {
		tokens->emplace_back(lines, CHAR, std::string(1, esc_char()));
		expect('\'', "Expect \" ' \" after Char");
	}
}

void Scanner::scan_identifier(std::vector<Token> *tokens) {
	while (!is_end() && is_letterN_Nnumber(cur_char())) {
		current++;
	}
	tokens->emplace_back(lines, IDENTIFIER,
	                     bytes->substr(start, current - start));
	return;
}

} // namespace scanner
} // namespace cbycpp
