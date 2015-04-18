#include "codeeditorwidget.h"
#include "syntaxhighlighter.h"
#include "linenumberarea.h"
#include <QPainter>
#include <QTextBlock>

CodeEditorWidget::CodeEditorWidget(QWidget* parent) :
	QPlainTextEdit(parent)
{
	lineNumberArea = new LineNumberArea(this);

	// Modify editor color and font settings
	QColor backgroundColor = QColor(39,40,34);
	QPalette colors = palette();
	colors.setColor(QPalette::Active, QPalette::Base, backgroundColor);
	colors.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
	colors.setColor(QPalette::Text, Qt::white);
	setPalette(colors);

	QFont font = QFont("Consolas", 10);
	font.setBold(true);
	setFont(font);

	// Syntax highlighting. If you wish to turn off syntax highlighting
	// at the moment only way to do so is to not initialize the member.
	// Will work on getting up more control over this in future updates.
	syntaxHighlighter = new SyntaxHighlighter(document());

	createConnections();
	updateLineNumberAreaWidth();
}

// Paints line number area off to the left
void CodeEditorWidget::lineNumberPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), QColor(59,58,50).lighter(160)); // TODO: Maybe get rid of magic number for color?

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
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

// Overload needed to handle resizing the line number area which is manually painted on.
void CodeEditorWidget::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect contRect = contentsRect();
	lineNumberArea->setGeometry(QRect(contRect.left(), contRect.top(), lineNumberAreaWidth(), contRect.height()));
}

// Creates signal/slot connections
void CodeEditorWidget::createConnections()
{
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
}

// Helper function for line number area
int CodeEditorWidget::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10)
	{
		max /= 10;
		++digits;
	}

	int space = 5 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

void CodeEditorWidget::updateLineNumberAreaWidth()
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

// Called to highlight the current line
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
		updateLineNumberAreaWidth();
}

