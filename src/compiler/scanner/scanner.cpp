#include <exception.hpp>
#include <iostream>
#include <scanner.hpp>

namespace cbycpp {
namespace scanner {
void Hello() { std::cout << "Hello" << std::endl; }

Token::Token(int line, TokenTypes type, std::string lexeme)
    : lines(line), type(type), lexeme(lexeme) {}

std::string Token::toString() const {
	// std::string literal_str = " ";
	// if (this->literal)
	// 	literal_str += this->literal.value().to_string() + " ";
	return "[" + this->getTypeString() + " \"" + this->lexeme + "\"" +
	       " line:" + std::to_string(this->lines) + "]";
}

std::string Token::getTypeString() const { return type_map.at(this->type); }

Scanner::Scanner(const icu::Normalizer2 *nfc) : nfc(nfc) {}

std::unique_ptr<Scanner::vTokens> Scanner::scan(std::string &b) {
	vTokens *tokens = new vTokens;
	auto u16str = UnicodeUtil::getU16String(b);
	bytes = &u16str;
	lines = 0, start = 0, current = 0;
	while (!is_end()) {
		start = current;
		UChar32 c = cur_advance();
		try {
			switch (c) {
			case U'+':
				tokens->emplace_back(lines, PLUS, "+");
				break;
			case U'-':
				tokens->emplace_back(lines, MINUS, "-");
				break;
			case U'*':
				tokens->emplace_back(lines, STAR, "*");
				break;
			case U'/':
				tokens->emplace_back(lines, SLASH, "/");
				break;
			case U'(':
				tokens->emplace_back(lines, LEFT_PAREN, "(");
				break;
			case U')':
				tokens->emplace_back(lines, RIGHT_PAREN, ")");
				break;
			case U'<':
				if (match(U'=')) {
					tokens->emplace_back(lines, LESS_EQUAL, "<=");
				} else {
					tokens->emplace_back(lines, LESS, "<");
				}
				break;
			case U'>':
				if (match(U'=')) {
					tokens->emplace_back(lines, GREATER_EQUAL, ">=");
				} else {
					tokens->emplace_back(lines, GREATER, ">");
				}
				break;
			case U'=':
				if (match(U'=')) {
					tokens->emplace_back(lines, EQUAL_EQUAL, "==");
				} else {
					tokens->emplace_back(lines, EQUAL, "=");
				}
				break;
			case U'!':
				if (match(U'=')) {
					tokens->emplace_back(lines, BANG_EQUAL, "!=");
				} else {
					tokens->emplace_back(lines, BANG, "!");
				}
				break;
			case U'"':
				scan_string(tokens);
				break;
			case U'\'':
				scan_char(tokens);
				break;
			case U'\n':
				lines++;
			case U'\t':
			case U' ':
				break;
			default:
				if (is_number(c)) {
					scan_number(tokens);
					break;
				} else if (UnicodeUtil::isXID_Start(c)) {
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
	tokens->emplace_back(lines, END, "");
	return std::unique_ptr<vTokens>(tokens);
}

void Scanner::scan_number(vTokens *tokens) {
	while (!is_end() && is_number(cur_char())) {
	    advance(cur_char());
	}
	if (!is_end() && match(U'.')) {
		while (!is_end() && is_number(cur_char())) {
		    advance(cur_char());
		}
		tokens->emplace_back(lines, FLOAT,
		                     UnicodeUtil::getU8String(
		                         bytes->tempSubString(start, current - start)));
		return;
	}
	// std::string lexeme = bytes->substr(start, current - start);
	// std::uint32_t value = std::stoi(lexeme);
	// std::optional<Variant> variant = Variant(&value, Variant::Type::I32);
	tokens->emplace_back(
	    lines, INT,
	    UnicodeUtil::getU8String(bytes->tempSubString(start, current - start)));
	return;
}

void Scanner::scan_string(std::vector<Token> *tokens) {
	icu::UnicodeString str{};
	bool has_dquote = false;
	while (!is_end()) {
		UChar32 c = esc_char();
		if (c == U'"') {
			has_dquote = true;
			break;
		}
		str += c;
	}
	if (!has_dquote)
		throw exception::ScanException(U'\0', lines,
		                               "Expect ' \" ' after String");
	tokens->emplace_back(lines, STRING, UnicodeUtil::getU8String(str));
	return;
}

void Scanner::scan_char(std::vector<Token> *tokens) {
	if (!is_end()) {
		tokens->emplace_back(lines, CHAR, UnicodeUtil::getU8String(esc_char()));
		expect(U'\'', "Expect \" ' \" after Char");
	}
}

void Scanner::scan_identifier(std::vector<Token> *tokens) {
	while (!is_end() && UnicodeUtil::isXID_Continue(cur_char())) {
		advance(cur_char());
	}
	if (match_keyword(tokens)) {
		return;
	} else {
		tokens->emplace_back(lines, IDENTIFIER,
		                     UnicodeUtil::getU8String(
		                         bytes->tempSubString(start, current - start)));
		return;
	}
}

bool Scanner::match_keyword(std::vector<Token> *tokens) {
	auto it = keywords_map.find(
	    UnicodeUtil::getU8String(bytes->tempSubString(start, current - start)));
	if (it == keywords_map.end()) {
		return false;
	} else {
		tokens->emplace_back(lines, it->second, it->first);
		return true;
	}
}

inline void Scanner::expect(UChar32 c, std::string msg) {
	if (!match(c))
		throw exception::ScanException(cur_char(), lines, msg);
}

} // namespace scanner
} // namespace cbycpp
