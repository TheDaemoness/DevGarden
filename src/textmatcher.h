#ifndef TEXTRULE_H
#define TEXTRULE_H

#include <QRegExp>
#include <QSyntaxHighlighter>

#include <vector>

/**
 * @brief QSyntaxHighlighter-friendly text matcher. Remember kids, don't play with matches.
 */
class TextMatcher {
	struct Rule {QRegExp rule; int flag;};
	struct MetadataRule {
		int posflags;
		int negflags;
		std::vector<Rule> beg;
		std::vector<Rule> end;
	};

public:
	void doMatching(QSyntaxHighlighter& hl);
private:
	std::vector<Rule> rules;
	std::vector<MetadataRule> metarules;
};

#endif // TEXTRULE_H
