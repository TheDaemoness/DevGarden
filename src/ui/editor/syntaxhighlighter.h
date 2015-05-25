#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include "../../textmatcher.h"

class SyntaxHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	explicit SyntaxHighlighter(QObject *parent = 0);

protected:
	void highlightBlock(const QString& text) Q_DECL_OVERRIDE;

	struct HighlightRule : public TextMatcher {
		QColor color;
		QTextFormat format;
	};

signals:

public slots:

private:


};

#endif // SYNTAXHIGHLIGHTER_H
