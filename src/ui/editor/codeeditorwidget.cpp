#include "codeeditorwidget.h"
#include "syntaxhighlighter.h"
#include "linenumberarea.h"
#include <QPainter>
#include <QTextBlock>

CodeEditorWidget::CodeEditorWidget(QWidget* parent)
{
	lineNumberArea = new LineNumberArea(this);

	// Connections
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	updateLineNumberAreaWidth(0);

	// Modify background/text of the editor
	QColor backgroundColor = QColor(39,40,34);
	QPalette colors = palette();
	colors.setColor(QPalette::Active, QPalette::Base, backgroundColor);
	colors.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
	colors.setColor(QPalette::Text, Qt::white);
	setPalette(colors);

	setFont(QFont("Consolas", 10));

	// Syntax highlighting
	syntaxHighlighter = new SyntaxHighlighter(document());
}

void CodeEditorWidget::lineNumberPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), QColor(59,58,50).lighter(160));

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + static_cast<int>(blockBoundingRect(block).height());

	while (block.isValid() && top <= event->rect().bottom())
	{
		if (block.isVisible() && bottom >= event->rect().top())
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(QColor(188,188,188));
			painter.setFont(QFont("Consolas", 10));
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + static_cast<int>(blockBoundingRect(block).height());
		++blockNumber;
	}
}

void CodeEditorWidget::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect contRect = contentsRect();
	lineNumberArea->setGeometry(QRect(contRect.left(), contRect.top(), lineNumberAreaWidth(), contRect.height()));
}

int CodeEditorWidget::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10)
	{
		max /= 10;
		++digits;
	}

	int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

void CodeEditorWidget::updateLineNumberAreaWidth(int newBlockCount)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditorWidget::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly())
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(73,72,62);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}

void CodeEditorWidget::updateLineNumberArea(const QRect& rect, int dy)
{
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}

