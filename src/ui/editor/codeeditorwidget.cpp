#include "codeeditorwidget.h"
#include "syntaxhighlighter.h"
#include "linenumberarea.h"

#include "../dgstyle.h"

#include <QPainter>
#include <QTextBlock>
#include <QFontDatabase>
#include <QKeyEvent>

#include "../../configloader.h"

CodeEditorWidget::CodeEditorWidget(QWidget* parent) :
	QPlainTextEdit(parent)
{

	lineNumberArea = new LineNumberArea(this);

	// Modify editor color and font settings
	QPalette colors = palette();
	colors.setColor(QPalette::Active, QPalette::Base, DGStyle::COLOR_BACKGROUND);
	colors.setColor(QPalette::Inactive, QPalette::Base, DGStyle::COLOR_BACKGROUND);
	colors.setColor(QPalette::Text, Qt::white);
	setPalette(colors);

	textFont.setStyleHint(QFont::Monospace);
	textFont.setPointSize(12);
	this->setFont(textFont);

	indent_primary = 4;
	indent_secondary = 4;

	//this->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont)); //Requires Qt 5.2

	// Syntax highlighting. If you wish to turn off syntax highlighting
	// at the moment only way to do so is to not initialize the member.
	// Will work on getting up more control over this in future updates.
	syntaxHighlighter = new SyntaxHighlighter(document());

	createConnections();
	updateLineNumberAreaWidth();
}

void CodeEditorWidget::keyPressEvent(QKeyEvent* key) {
	if(key->key() == Qt::Key_Tab) {
		if(indent_primary) {
			uint8_t indent_level = (indent_primary-this->textCursor().columnNumber()%indent_primary);
			this->textCursor().insertText(QString(indent_level, ' '));
		} else
			this->textCursor().insertText("\t");
	}
	else
		QPlainTextEdit::keyPressEvent(key);
}

// Paints line number area off to the left
void CodeEditorWidget::lineNumberPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), DGStyle::COLOR_LOLIGHT.lighter(160)); // TODO: Maybe get rid of magic number for color?

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
			painter.setFont(textFont);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + static_cast<int>(blockBoundingRect(block).height());
		++blockNumber;
	}
}

void CodeEditorWidget::configure(ConfigFile& cfg) {
	ConfigEntry* a = cfg.at("indent-primary");
	ConfigEntry* b = cfg.at("indent-secondary");
	if(a) {
		DEBUG_EMIT("A");
		a->split();
		parseConfigEntry(*a,indent_primary);
	}
	if(b) {
		DEBUG_EMIT("B");
		b->split();
		parseConfigEntry(*b,indent_secondary);
	}
}

void CodeEditorWidget::parseConfigEntry(const ConfigEntry& data, uint8_t& field) {
	const QString* word;
	word = data.getData(1);
	if(word) {
		if(*word == "tab")
			field = 0;
		else if(*word == "space") {
			word = data.getData(2);
			if(word) {
				uint8_t val = word->toUShort();
				field = val?val:4;
			} else
				field = 4;
		}
	}
}

void CodeEditorWidget::fontSizeInc() {
	if(textFont.pointSize() >= 12)
		textFont.setPointSize(textFont.pointSize()+2);
	else
		textFont.setPointSize(textFont.pointSize()+1);
	this->setFont(textFont);
}

void CodeEditorWidget::fontSizeDec() {
	if(textFont.pointSize() > 12)
		textFont.setPointSize(textFont.pointSize()-2);
	else if(textFont.pointSize() > 8)
		textFont.setPointSize(textFont.pointSize()-1);
	this->setFont(textFont);
}

void CodeEditorWidget::fontSizeRes() {
	if(textFont.pointSize() > 12)
		textFont.setPointSize(12);
	this->setFont(textFont);
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
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth()));
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

		selection.format.setBackground(DGStyle::COLOR_LOLIGHT);
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

