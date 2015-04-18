#ifndef LINE_NUMBER_AREA
#define LINE_NUMBER_AREA

#include <QWidget>
#include <QSize>
#include "codeeditorwidget.h"

class LineNumberArea : public QWidget
{
	Q_OBJECT

	public:
		LineNumberArea(CodeEditorWidget* editor) : QWidget(editor), codeEditor(editor) {}

		QSize sizeHint() const Q_DECL_OVERRIDE
		{
			return QSize(codeEditor->lineNumberAreaWidth(), 0);
		}

	protected:
		void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE
		{
			codeEditor->lineNumberPaintEvent(event);
		}

	private:
		CodeEditorWidget* codeEditor;
};

#endif // LINE_NUMBER_AREA

