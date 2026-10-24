#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unicode.hpp>
#include <unordered_map>
#include <variant.hpp>
#include <vector>

#include <iostream>

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
	TRUE,
	FALSE,
	NIL,

	END,

	WRITE,
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
    {TRUE, "TRUE"},
    {FALSE, "FALSE"},
    {NIL, "NULL"},
    {END, "EOF"},
    {WRITE, "WRITE"},
};

const std::unordered_map<std::string, TokenTypes> keywords_map = {
    {"true", TRUE},
    {"false", FALSE},
    {"null", NIL},
    {"Write", WRITE},
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
	std::string toString() const;
	std::string getTypeString() const;
};

class Scanner {
  private:
	inline bool is_number(UChar32 c) const;
	inline bool is_letterN_(UChar32 c) const;
	inline bool is_letterN_Nnumber(UChar32 c) const;
	inline bool is_end() const;

	void scan_number(UChar32 first, std::vector<Token> *tokens);
	void scan_string(std::vector<Token> *tokens);
	void scan_char(std::vector<Token> *tokens);
	void scan_identifier(UChar32 first, std::vector<Token> *tokens);
	bool match_keyword(icu::UnicodeString ustr, std::vector<Token> *tokens);

	// inline UChar32 advance(UChar32 c);
	inline UChar32 cur_advance();
	inline UChar32 cur_char();
	inline bool match(UChar32 c);
	inline UChar32 esc_char();
	inline void expect(UChar32 c, std::string msg);

	std::size_t lines{};
	// std::size_t start{}, current{};
	std::unique_ptr<UnicodeUtil::U32It> pU32It{};
	std::unique_ptr<UnicodeUtil::U32It> pU32End{};

	const icu::Normalizer2 *nfc;

  public:
	Scanner(const icu::Normalizer2 *nfc);
	using vTokens = std::vector<Token>;
	std::unique_ptr<vTokens> scan(std::string &input_bytes);
};

inline bool Scanner::is_number(UChar32 c) const {
	if (c >= U'0' && c <= U'9')
		return true;
	else
		return false;
}

inline bool Scanner::is_letterN_(UChar32 c) const {
	if ((c >= U'A' && c <= U'Z') || (c >= U'a' && c <= U'z') || c == U'_')
		return true;
	else
		return false;
}

inline bool Scanner::is_letterN_Nnumber(UChar32 c) const {
	if (is_number(c) || is_letterN_(c))
		return true;
	else
		return false;
	;
}

inline bool Scanner::is_end() const {
	if (*pU32It != *pU32End)
		return false;
	else
		return true;
}

// inline UChar32 Scanner::advance(UChar32 c) {
// 	current += U16_LENGTH(c);
// 	return c;
// }

inline UChar32 Scanner::cur_advance() {
	if (is_end())
		return U'\0';
	else {
		return (*((*pU32It)++)).codePoint();
	}
}

inline UChar32 Scanner::cur_char() {
	if (is_end())
		return U'\0';
	else
		return (**pU32It).codePoint();
}

inline bool Scanner::match(UChar32 c) {
	if (!is_end() && cur_char() == c) {
		(*pU32It)++;
		return true;
	} else
		return false;
}

inline UChar32 Scanner::esc_char() {
	if (match(U'\\')) {
		switch (cur_advance()) {
		case U'n':
			return U'\n';
		case U't':
			return U'\t';
		case U'"':
			return U'"';
		case U'\'':
			return U'\'';
		case U'\\':
			return U'\\';
		default:
			break;
		}
	}
	return cur_advance();
}

} // namespace scanner
} // namespace cbycpp
