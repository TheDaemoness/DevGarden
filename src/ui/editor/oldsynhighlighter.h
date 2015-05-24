#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include "codestyle.h"

#include <QSyntaxHighlighter>

/**
 * @brief Incomplete temporary C++ syntax highlighter.
 * @deprecated Will be replaced with a configurable system later.
 */
class SyntaxHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	SyntaxHighlighter(QTextDocument* parent = 0);

protected:
	void highlightBlock(const QString& text) Q_DECL_OVERRIDE;

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

	QVector<HighlightingRule> highlightingRules;

	QRegExp commentStartExp;
	QRegExp commentEndExp;

	QTextCharFormat keywordFormat;
	QColor keywordColor;

	QTextCharFormat integerFormat;
	QColor integerColor;

	QTextCharFormat includeHeaderFormat;
	QColor headerColor;

	QTextCharFormat extraKeywordFormat;
	QColor extraKeywordColor;

	QTextCharFormat classFormat;
	QColor classColor;

	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QColor commentColor;

	QTextCharFormat quotationFormat;
	QColor quotationColor;

	QTextCharFormat functionFormat;
	QColor functionColor;
};

#endif // SYNTAX_HIGHLIGHTER_H
