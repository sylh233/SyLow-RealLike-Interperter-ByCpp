#pragma once

#include <cstddef>
#include <cstdint>
#include <exception.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant.hpp>
#include <vector>

namespace cbycpp {
namespace scanner {
void Hello();

enum TokenTypes {
	PLUS,
	MINUS,
	STAR,
	SLASH, // +-*/
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	EQUAL,
	EQUAL_EQUAL,
	BANG,
	BANG_EQUAL,

	LEFT_PAREN,
	RIGHT_PAREN, // (,)

	INT,
	FLOAT,
	CHAR,
	STRING,
	IDENTIFIER,
};

const std::unordered_map<TokenTypes, std::string> type_map = {
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {STAR, "STAR"},
    {SLASH, "SLASH"},
    {LEFT_PAREN, "L_PAREN"},
    {RIGHT_PAREN, "R_PAREN"},
    {INT, "INT"},
    {FLOAT, "FLOAT"},
    {CHAR, "CHAR"},
    {STRING, "STRING"},
    {IDENTIFIER, "IDENTIFIER"},
    {LESS, "LESS"},
    {LESS_EQUAL, "L_EQUAL"},
    {GREATER, "GREATER"},
    {GREATER_EQUAL, "G_EQUAL"},
    {EQUAL, "EQUAL"},
    {EQUAL_EQUAL, "E_EQUAL"},
    {BANG, "BANG"},
    {BANG_EQUAL, "B_EQUAL"},
};

class Token {
  private:
	// const std::optional<Variant> literal;

  public:
	const int lines;
	const TokenTypes type;
	const std::string lexeme;

	Token(int lines, TokenTypes type, std::string lexeme);
	// std::optional<Variant> literal = std::nullopt);
	std::string toString();
	std::string getTypeString();
};

class Scanner {
  private:
	inline bool is_number(char c) const;
	inline bool is_letterN_(char c) const;
	inline bool is_letterN_Nnumber(char c) const;
	inline bool is_end() const;

	void scan_number(std::vector<Token> *tokens);
	void scan_string(std::vector<Token> *tokens);
	void scan_char(std::vector<Token> *tokens);
	void scan_identifier(std::vector<Token> *tokens);

	inline char cur_advance();
	inline char cur_char();
	inline bool match(char c);
	inline char esc_char();
	inline void expect(char c, std::string msg);

	std::size_t lines{};
	std::size_t start{}, current{};
	std::string *bytes;

  public:
	using vTokens = std::vector<Token>;
	std::unique_ptr<vTokens> scan(std::string &input_bytes);
};

inline bool Scanner::is_number(char c) const {
	if (c >= '0' && c <= '9')
		return true;
	else
		return false;
}

inline bool Scanner::is_letterN_(char c) const {
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
		return true;
	else
		return false;
}

inline bool Scanner::is_letterN_Nnumber(char c) const {
	if (is_number(c) && is_letterN_(c))
		return true;
	else
		return false;
	;
}

inline bool Scanner::is_end() const {
	if (current < bytes->length())
		return false;
	else
		return true;
}

inline char Scanner::cur_advance() { return bytes->at(current++); }

inline char Scanner::cur_char() { return bytes->at(current); }

inline bool Scanner::match(char c) {
	if (!is_end() && cur_char() == c) {
		current++;
		return true;
	} else
		return false;
}

inline char Scanner::esc_char() {
	if (match('\\')) {
		switch (cur_advance()) {
		case 'n':
			return '\n';
		case 't':
			return '\t';
		case '"':
			return '"';
		case '\'':
			return '\'';
		case '\\':
			return '\\';
		default:
			break;
		}
	}
	return cur_advance();
}

inline void Scanner::expect(char c, std::string msg) {
	if (!match(c))
		throw exception::ScanException(cur_char(), lines, msg);
}

} // namespace scanner
} // namespace cbycpp
