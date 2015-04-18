#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include <QSyntaxHighlighter>

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
		QTextCharFormat integerFormat;
		QTextCharFormat includeHeaderFormat;
		QTextCharFormat extraKeywordFormat;
		QTextCharFormat classFormat;
		QTextCharFormat singleLineCommentFormat;
		QTextCharFormat multiLineCommentFormat;
		QTextCharFormat quotationFormat;
		QTextCharFormat functionFormat;
};

#endif // SYNTAX_HIGHLIGHTER_H
