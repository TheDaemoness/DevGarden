#include "dgcentralwidget.hpp"
#include "editor/codeeditorwidget.h"

#include <QFileSystemModel>
#include <QCoreApplication>
#include <QTreeView>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

DGCentralWidget::DGCentralWidget(QWidget *parent) :
	QWidget(parent)
{
	createWidgets();
	createLayout();
}

void DGCentralWidget::createWidgets()
{
	// Project Directory Tree
	projectDirModel = new QFileSystemModel;
	projectDirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	projectDirModel->setRootPath(QCoreApplication::applicationDirPath());

	projectDirView = new QTreeView;
	projectDirView->setModel(projectDirModel);
	projectDirView->setColumnHidden(1, true);
	projectDirView->setColumnHidden(2, true);
	projectDirView->setColumnHidden(3, true);

	// Auxiliary ComboBox
	auxComboBox = new QComboBox;
	auxComboBox->addItem("Aux ComboBox");

	// Auxiliary Pane
	auxPane = new QPlainTextEdit;
	auxPane->setPlainText("Auxiliary\nPane\nPlaceholder");

	// Text Editor
	textEditor = new CodeEditorWidget;
	textEditor->setPlainText("#include <iostream>\n\n"
							 "int main()\n"
							 "{\n"
							 "    std::cout << \"dtscode is a nim noob\";\n"
							 "    return 0;\n"
							 "}");

	// Bottom Bar
	bottomBar = new QLineEdit;
	bottomBar->setText("Regen/Build/Clean/Run/Debug/Setup - File Info");
	bottomBar->setAlignment(Qt::AlignCenter);

	// Split View Options Pane
	splitViewPane = new QPlainTextEdit;
	splitViewPane->setPlainText("Split\nView\nOptions\nPane");

	// Bottom Push Button
	bottomButton = new QPushButton("O"); // Placeholder until an QIcon can be created
}

void DGCentralWidget::createLayout()
{
	// I am using stretch values at the moment for ease of use only.
	// Later on will be switching to size policies to make the sizing right.

	// ProjectDir, AuxCombo, AuxPane
	QVBoxLayout* leftSideLayout = new QVBoxLayout;
	leftSideLayout->addWidget(projectDirView, 3);
	leftSideLayout->addWidget(auxComboBox, 1);
	leftSideLayout->addWidget(auxPane, 3);

	// TextEditor, BottomBar
	QVBoxLayout* centralLayout = new QVBoxLayout;
	centralLayout->addWidget(textEditor, 5);
	centralLayout->addWidget(bottomBar, 1);

	// SplitViewPane, BottomButton
	QVBoxLayout* rightSideLayout = new QVBoxLayout;
	rightSideLayout->addWidget(splitViewPane, 5);
	rightSideLayout->addWidget(bottomButton, 1);

	// Main Layout (Combination of all child layouts)
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->addLayout(leftSideLayout, 3);
	mainLayout->addLayout(centralLayout, 20);
	mainLayout->addLayout(rightSideLayout, 1);

	setLayout(mainLayout);
}
