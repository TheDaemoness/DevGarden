#include "dgcentralwidget.hpp"
#include "editor/codeeditorwidget.h"

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
#include <QGridLayout>
#include <QSpacerItem>

#include <QHeaderView>
#include <QScrollBar>

#include "../dgcontroller.h"
#include "../envmacros.h"

DGCentralWidget::DGCentralWidget(DGController* ctrl, QWidget *parent) :
	QWidget(parent), ctrl(ctrl)
{
	createWidgets();
	createLayout();
	setupConnections();
}

QPushButton* DGCentralWidget::makeButton(const QString& txt, int width, int height) {
	auto* retval = new QPushButton(txt);
	retval->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	retval->setMinimumSize(width, height);
	retval->setMaximumSize(width, height);
	return retval;
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
	projectDirView->setHidden(true);

	// Auxiliary ComboBox
	auxComboBox = new QComboBox;
	auxComboBox->addItem("Aux ComboBox");
	auxComboBox->hide();

	// Project
	projectComboBox = new QComboBox;
	projectComboBox->addItem("Projects");
	projectComboBox->hide();
	this->connect(this->projectComboBox,SIGNAL(currentIndexChanged(int)),SLOT(changeProject(int)));

	// Auxiliary Pane
	auxPane = new QListWidget();
	auxPane->addItem("Auxiliary Pane!");
	auxPane->hide();

	// Text Editor
	textEditor = new CodeEditorWidget;
	textEditor->setPlainText("#include <stdio.h>\n\n"
							 "int main(int argc, char** argv) {\n"
							 "    printf(\"dtscode is a nim cohort\\n\");\n"
							 "    return 0;\n"
							 "}");

	// Bottom Push Button
	bottomButton = makeButton("CMD", 48, 32);

	// Bottom Bar
	bottomBar = new QHBoxLayout();
	bottomBar->addWidget(makeButton("Regen", 64, 32));
	bottomBar->addWidget(makeButton("Build", 64, 32));
	bottomBar->addWidget(makeButton("Clean", 64, 32));
	bottomBar->addWidget(makeButton("Run", 64, 32));
	bottomBar->addWidget(makeButton("Install", 64, 32));
	bottomBar->addWidget(makeButton("Setup", 64, 32));
	bottomBar->setSpacing(4);
	bottomBar->addStretch();
	bottomBar->addWidget(bottomButton);

	// Split View Options Pane
	splitViewPane = new QVBoxLayout();
	splitViewPane->setSpacing(4);
	splitViewPane->addWidget(makeButton("Code", 48));
	splitViewPane->addWidget(makeButton("Docs", 48));
	splitViewPane->addStretch();
}

void DGCentralWidget::createLayout()
{
	// I am using stretch values at the moment for ease of use only.
	// Later on will be switching to size policies to make the sizing right.

	// ProjectDir, AuxCombo, AuxPane
	leftSideLayout = new QVBoxLayout;
	leftSideLayout->setSpacing(4);
	leftSideLayout->addWidget(projectComboBox, 1);
	leftSideLayout->addWidget(projectDirView, 3);
	leftSideLayout->addWidget(auxComboBox, 1);
	leftSideLayout->addWidget(auxPane, 3);
	leftSideLayout->update();

	// TextEditor, BottomBar
	QHBoxLayout* centralLayout = new QHBoxLayout;
	centralLayout->setSpacing(4);
	centralLayout->addWidget(textEditor);
	centralLayout->addLayout(splitViewPane);

	QVBoxLayout* rightSideLayout = new QVBoxLayout;
	centralLayout->setSpacing(4);
	rightSideLayout->addLayout(centralLayout);
	rightSideLayout->addLayout(bottomBar);

	// Main Layout (Combination of all child layouts)
	mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(4);
	mainLayout->addLayout(leftSideLayout);
	mainLayout->addLayout(rightSideLayout, 20);

	setLayout(mainLayout);
}

void DGCentralWidget::setupConnections() {
	this->connect(projectDirView,   SIGNAL(expanded(QModelIndex)),              SLOT(resizeDirView()));
	this->connect(projectDirView,   SIGNAL(collapsed(QModelIndex)),             SLOT(resizeDirView()));
	this->connect(projectComboBox,  SIGNAL(currentIndexChanged(int)),           SLOT(changeProject(int)));
	this->connect(ctrl,             SIGNAL(sigProjectListChanged()),            SLOT(updateProjectList()));
	this->connect(ctrl,             SIGNAL(sigProjectClosed()),                 SLOT(shrinkProjectList()));
	this->connect(projectDirView,   SIGNAL(clicked(const QModelIndex&)),        SLOT(changeFile(const QModelIndex&)));

	connect(getEditor(), SIGNAL(textChanged()), ctrl, SLOT(fileEdited()));
}

void DGCentralWidget::resizeDirView() {
	projectDirView->resizeColumnToContents(0);
}

void DGCentralWidget::changeProject(int index) {
	projectDirView->setModel(nullptr);
	QString root = ctrl->changeProject(index);
	QFileSystemModel* m = ctrl->getActiveProjectModel();
	if(m) {
		projectDirView->setModel(m);
		projectDirView->setRootIndex(m->index(root));
		projectDirView->setColumnHidden(1, true);
		projectDirView->setColumnHidden(2, true);
		projectDirView->setColumnHidden(3, true);
		resizeDirView();
	} else
		ctrl->getFile(ctrl->getPath());
	projectDirView->setHidden(!m);
	static_cast<QWidget*>(this->parent())->setWindowTitle(QString(DG_NAME) + " - " + projectComboBox->currentText());
}

void DGCentralWidget::shrinkProjectList() {
	if(!ctrl->getProjects().isEmpty())
		this->projectComboBox->removeItem(this->projectComboBox->currentIndex());
	else {
		this->projectComboBox->clear();
		projectDirView->setModel(nullptr);
		projectDirView->setHidden(true);
		static_cast<QWidget*>(this->parent())->setWindowTitle(QString(DG_NAME));
	}
	size_t len = this->ctrl->getProjects().length();

	this->projectComboBox->setHidden(len < 2);

	//LEFT PANE HIDING.
	this->auxComboBox->setHidden(len < 1);
	this->auxPane->setHidden(len < 1);
	this->setHiddenLeft(len < 1);
}

void DGCentralWidget::changeFile(const QModelIndex& val) {
	if(projectDirModel->isDir(val))
		return;
	ctrl->getFile(projectDirModel->filePath(val));
	QString title = QString(DG_NAME) + " - " + projectComboBox->currentText();
	if(this->ctrl->getActiveProjectModel())
		title += " - " + projectDirModel->fileName(val);
	static_cast<QWidget*>(this->parent())->setWindowTitle(title);
}

void DGCentralWidget::updateProjectList() {
	this->projectComboBox->clear();
	this->projectComboBox->blockSignals(true);
	if(!ctrl->getProjects().isEmpty()) {
		this->projectComboBox->addItems(ctrl->getProjects());
		this->projectComboBox->setCurrentIndex(this->ctrl->getProjects().length()-1);
		this->changeProject(this->ctrl->getProjects().length()-1);
	} else {
		projectDirView->setModel(nullptr);
		projectDirView->setHidden(true);
		static_cast<QWidget*>(this->parent())->setWindowTitle(QString(DG_NAME));
	}
	this->projectComboBox->blockSignals(false);
	size_t len = this->ctrl->getProjects().length();

	this->projectComboBox->setHidden(len < 2);

	this->auxComboBox->setHidden(len < 1);
	this->auxPane->setHidden(len < 1);
	this->setHiddenLeft(len < 1);
}

void DGCentralWidget::setHiddenLeft(bool hide) {
	mainLayout->setStretch(0,hide?0:5);
}


