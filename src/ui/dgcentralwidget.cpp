#include "dgcentralwidget.hpp"

#include <QFileSystemModel>
#include <QCoreApplication>
#include <QTreeView>
#include <QListWidget>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QHeaderView>
#include <QScrollBar>

#include "../dgcontroller.h"

DGCentralWidget::DGCentralWidget(DGController* ctrl, QWidget *parent) :
	QWidget(parent), ctrl(ctrl)
{
	createWidgets();
	createLayout();
	setupConnections();
}

void DGCentralWidget::createWidgets()
{
	// Project Directory Tree
	projectDirModel = new QFileSystemModel;
	projectDirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	projectDirModel->setRootPath(QCoreApplication::applicationDirPath());

	projectDirView = new QTreeView;
	projectDirView->setModel(projectDirModel);
	projectDirView->setWordWrap(false);
	projectDirView->setTextElideMode(Qt::ElideNone);
	projectDirView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
	projectDirView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	projectDirView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	projectDirView->header()->hide();
	projectDirView->setColumnHidden(1, true);
	projectDirView->setColumnHidden(2, true);
	projectDirView->setColumnHidden(3, true);
	projectDirView->setHidden(true);

	// Auxiliary ComboBox
	auxComboBox = new QComboBox;
	auxComboBox->addItem("Aux ComboBox");

	// Project
	projectComboBox = new QComboBox;
	projectComboBox->addItem("Projects");
	this->connect(this->projectComboBox,SIGNAL(currentIndexChanged(QString)),SLOT(changeProject(QString)));

	// Auxiliary Pane
	auxPane = new QListWidget();
	auxPane->addItem("Auxiliary Pane!");

	// Text Editor
	textEditor = new QPlainTextEdit;
	textEditor->setPlainText("#include <iostream>\n\n"
							 "int main()\n"
							 "{\n"
							 "    std::cout << \"dtscode is a nim consort\";\n"
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
	leftSideLayout->addWidget(projectComboBox, 1);
	leftSideLayout->addWidget(projectDirView, 3);
	leftSideLayout->addWidget(auxComboBox, 1);
	leftSideLayout->addWidget(auxPane, 3);

	// TextEditor, BottomBar
	QVBoxLayout* centralLayout = new QVBoxLayout;
	centralLayout->addWidget(textEditor, 6);
	centralLayout->addWidget(bottomBar, 2);

	// SplitViewPane, BottomButton
	QVBoxLayout* rightSideLayout = new QVBoxLayout;
	rightSideLayout->addWidget(splitViewPane, 7);
	rightSideLayout->addWidget(bottomButton, 1);

	// Main Layout (Combination of all child layouts)
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->addLayout(leftSideLayout, 3);
	mainLayout->addLayout(centralLayout, 20);
	mainLayout->addLayout(rightSideLayout, 1);

	setLayout(mainLayout);
}

void DGCentralWidget::setupConnections() {
	this->connect(projectDirView,SIGNAL(expanded(QModelIndex)),SLOT(resizeDirView()));
	this->connect(projectDirView,SIGNAL(collapsed(QModelIndex)),SLOT(resizeDirView()));
	this->connect(projectComboBox,SIGNAL(currentIndexChanged(QString)),SLOT(changeProject(QString)));
}

void DGCentralWidget::resizeDirView() {
	projectDirView->resizeColumnToContents(0);
}

void DGCentralWidget::changeProject(const QString &str) {
	ctrl->changeProject(str);
	QFileSystemModel* m = ctrl->getActiveProjectModel();
	if(m)
		projectDirView->setModel(m);
	projectDirView->setHidden(!m);
}
