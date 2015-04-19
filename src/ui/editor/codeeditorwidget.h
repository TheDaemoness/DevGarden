#ifndef CODEEDITORWIDGET_H
#define CODEEDITORWIDGET_H

#include <QObject>
#include <QPlainTextEdit>

class SyntaxHighlighter;

class CodeEditorWidget : public QPlainTextEdit
{
	Q_OBJECT

	public:
		CodeEditorWidget(QWidget* parent = 0);

		void lineNumberPaintEvent(QPaintEvent* event);
		int lineNumberAreaWidth();

	public slots:
		void fontSizeInc();
		void fontSizeDec();
		void fontSizeRes();

	protected:
		void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

	private:
		void createConnections();

	private slots:
		void updateLineNumberAreaWidth();
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect& rect, int dy);

	private:
		QFont textFont;
		QWidget* lineNumberArea;
		SyntaxHighlighter* syntaxHighlighter;
};

#endif // CODEEDITORWIDGET_H
