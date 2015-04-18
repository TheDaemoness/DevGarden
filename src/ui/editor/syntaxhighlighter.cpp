#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
	: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	keywordFormat.setForeground(QColor(102,217,239));
	QStringList keywordPatterns;
	keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
					<< "\\bdouble\\b" << "\\benum\\b"
					<< "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
					<< "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
					<< "\\bvolatile\\b"
					<< "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
					<< "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
					<< "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
					<< "\\bunion\\b" << "\\bunsigned\\b"
					<< "\\bvoid\\b";

	foreach (const QString& pattern, keywordPatterns)
	{
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	includeHeaderFormat.setForeground(QColor(220,213,131));
	rule.pattern = QRegExp("include ((<[^>]+>)|(\"[^\"]+\"))");
	rule.format = includeHeaderFormat;
	highlightingRules.append(rule);

	extraKeywordFormat.setForeground(QColor(185,38,76));
	QStringList extraKeywordPatterns;
	extraKeywordPatterns << "\\bprivate\\b" << "\\bpublic\\b" << "\\bprotected\\b"
						 << "\\bifndef\\b" << "\\bendif\\b" << "\\binclude\\b"
						 << "\\bvirtual\\b" << "\\binclude\\b" << "\\bdefine\\b"
						 << "\\bexplicit\\b";

	foreach (const QString& pattern, extraKeywordPatterns)
	{
		rule.pattern = QRegExp(pattern);
		rule.format = extraKeywordFormat;
		highlightingRules.append(rule);
	}

	integerFormat.setForeground(QColor(192,151,210));
	rule.pattern = QRegExp("[0-9]+");
	rule.format = integerFormat;
	highlightingRules.append(rule);

	classFormat.setForeground(Qt::white);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);

	quotationFormat.setForeground(QColor(230,219,103));
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	functionFormat.setFontItalic(true);
	functionFormat.setForeground(QColor(166,226,44));
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	singleLineCommentFormat.setForeground(QColor(104,102,83));
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(QColor(104,102,83).lighter(240));

	commentStartExp = QRegExp("/\\*");
	commentEndExp = QRegExp("\\*/");
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
	foreach (const HighlightingRule &rule, highlightingRules)
	{
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}

	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = commentStartExp.indexIn(text);

	while (startIndex >= 0)
	{
		int endIndex = commentEndExp.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1)
		{
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else
		{
			commentLength = endIndex - startIndex + commentEndExp.matchedLength();
		}

		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = commentStartExp.indexIn(text, startIndex + commentLength);
	}
}

