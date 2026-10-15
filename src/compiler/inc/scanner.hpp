#pragma once

#include <cstddef>
#include <cstdint>
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

	LEFT_PAREN,
	RIGHT_PAREN, // (,)

	NUM,
};

const std::unordered_map<TokenTypes, std::string> type_map = {
    {PLUS, "PLUS"},   {MINUS, "MINUS"},        {STAR, "STAR"},
    {SLASH, "SLASH"}, {LEFT_PAREN, "L_PAREN"}, {RIGHT_PAREN, "R_PAREN"},
    {NUM, "NUM"},
};

class Token {
  private:
	const int lines;
	const TokenTypes type;
	const std::string lexeme;
	const std::optional<Variant> literal;

  public:
	Token(int lines, TokenTypes type, std::string lexeme,
	      std::optional<Variant> literal = std::nullopt);
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
	inline char cur_advance();
	inline char cur_char();

	std::size_t lines{};
	std::size_t start{}, current{};
	std::string *bytes;

  public:
	std::vector<Token> *scan(std::string &input_bytes);
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
} // namespace scanner
} // namespace cbycpp
