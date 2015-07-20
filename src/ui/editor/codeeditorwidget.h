#ifndef CODEEDITORWIDGET_H
#define CODEEDITORWIDGET_H

#include <QObject>
#include <QPlainTextEdit>

class OldSyntaxHighlighter;
class QKeyEvent;

class ConfigFile;
class ConfigEntry;

/**
 * @brief The central code editor widget.
 */
class CodeEditorWidget : public QPlainTextEdit
{
	Q_OBJECT

	public:
		CodeEditorWidget(QWidget* parent = 0);

		void lineNumberPaintEvent(QPaintEvent* event);
		int lineNumberAreaWidth();

		void configure(ConfigFile& cfg);
		void setContents(const QString & text);

	public slots:
		void fontSizeInc();
		void fontSizeDec();
		void fontSizeRes();

	protected:
		void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
		void keyPressEvent(QKeyEvent* key) Q_DECL_OVERRIDE;

	private:
		void createConnections();
		void parseConfigEntry(const ConfigEntry& data, uint8_t& field);
		void indent(const uint8_t& lvl);
		void setTabWidth(uint8_t len);

	private slots:
		void updateLineNumberAreaWidth();
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect& rect, int dy);

	private:
		QFont textFont;
		QWidget* lineNumberArea;
		//OldSyntaxHighlighter* syntaxHighlighter;
		uint8_t indent_primary, indent_secondary, tab_width;
		bool spaced, tabbed;
};

#endif // CODEEDITORWIDGET_H
