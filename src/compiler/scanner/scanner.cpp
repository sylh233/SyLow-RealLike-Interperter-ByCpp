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
	while (current < bytes->length()) {
		start = current;
		char c = cur_advance();
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
		case '\n':
			lines++;
		case '\t':
		case ' ':
			break;
		default:
			if (is_number(c))
				scan_number(tokens);
			if (is_letterN_(c)) {
			}
			break;
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

} // namespace scanner
} // namespace cbycpp
