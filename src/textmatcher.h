#ifndef TEXTMATCHER_H
#define TEXTMATCHER_H

#include <vector>

#include <QRegExp>

class QSyntaxHighlighter;

/**
 * @brief A text matcher for use with QSyntaxHighlighters. Remember kids, don't play with matches.
 * Called by a QSyntaxHighligher to highlight a document.
 * Instantiated per language. Constructed by LangRegistry.
 */
class TextMatcher {
public:
	void doMatching(QSyntaxHighlighter& hl);
private:
	struct Matcher {QRegExp rule; int flag;};
	struct Rule {
		int posflags;
		int negflags;
		int color;
		std::vector<Matcher> beg;
		std::vector<Matcher> end;
	};
	std::vector<Rule> rules_color;
	std::vector<Rule> rules_block;
};

#endif // TEXTMATCHER_H
