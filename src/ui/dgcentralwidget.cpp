#include "dgcentralwidget.hpp"
#include "editor/codeeditorwidget.h"

#include "dgtaskstatuslabel.h"

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
#include <QLabel>
#include <QLineEdit>

#include <QHeaderView>
#include <QScrollBar>
#include <QSplitterHandle>
#include <QStackedLayout>
#include <QWindow>

#include "../dgcontroller.h"
#include "../envmacros.h"

DGCentralWidget::DGCentralWidget(DGController* ctrl, Executor* exe, QWidget *parent) :
	QWidget(parent), ctrl(ctrl)
{
	createWidgets(exe);
	createLayout();
	setupConnections();
}

QPushButton* DGCentralWidget::makeButton(const QString& txt, int width, int height) {
	auto* retval = new QPushButton(tr(txt.toLocal8Bit()));
	//retval->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	//retval->setMinimumSize(width, height);
	//retval->setMaximumSize(width, height);
	return retval;
}

void DGCentralWidget::createWidgets(Executor* exe)
{
	leftBar = new QWidget;

	// Project Directory Tree
	projectDirModel = new QFileSystemModel(this);
	projectDirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	projectDirModel->setRootPath(QCoreApplication::applicationDirPath());

	projectDirView = new QTreeView(leftBar);
	projectDirView->setModel(projectDirModel);
	projectDirView->setWordWrap(false);
	projectDirView->setTextElideMode(Qt::ElideNone);
	projectDirView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
	projectDirView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	projectDirView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	projectDirView->header()->hide();
	projectDirView->header()->geometriesChanged();
	projectDirView->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));

	// Auxiliary ComboBox
	auxComboBox = new QComboBox(leftBar);
	auxComboBox->addItem("Aux ComboBox");
	auxComboBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));

	// Project
	projectComboBox = new QComboBox(leftBar);
	projectComboBox->addItem("Projects");
	this->connect(this->projectComboBox,SIGNAL(currentIndexChanged(int)),SLOT(changeProject(int)));
	projectComboBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));

	// Auxiliary Pane
	auxPane = new QListWidget(leftBar);
	auxPane->addItem("Auxiliary Pane!");
	auxPane->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));

	// Text Editor
	textEditor = new CodeEditorWidget;

	//File Info Label
	fileInfo = new QLabel;
	fileInfo->setAlignment(Qt::AlignRight);

	//Command box.
	cmdLine = new QLineEdit;
	cmdLine->setPlaceholderText("Enter command...");

	taskStatusLabel = new DGTaskStatusLabel(exe);

	// Bottom Bar
	rightBarLayout = new QVBoxLayout;
	for(size_t i = 0; i < BUTTON_LOWER_NAMES.size(); ++i) {
		auto* button = makeButton(BUTTON_LOWER_NAMES[i], 72, 32);
		buttonsSide.emplace(static_cast<ButtonIdLower>(i), button);
		rightBarLayout->addWidget(button);
		button->setHidden(true);
	}
	buttonsSide[SETUP]->setHidden(false);
	rightBarLayout->setSpacing(4);
	rightBarLayout->addStretch();
}

void DGCentralWidget::createLayout()
{
	auxPanePair = new QVBoxLayout;
	auxPanePair->addWidget(auxComboBox);
	auxPanePair->addWidget(auxPane);
	auxPanePair->setContentsMargins(0,0,0,0);

	leftSplitter = new QSplitter(Qt::Vertical);
	leftSplitter->addWidget(projectDirView);
	QWidget *temp = new QFrame;
	temp->setLayout(auxPanePair);
	leftSplitter->addWidget(temp);

	// ProjectDir, AuxCombo, AuxPane
	leftSideLayout = new QVBoxLayout;
	leftSideLayout->setSpacing(4);
	leftSideLayout->addWidget(projectComboBox);
	leftSideLayout->addWidget(leftSplitter);
	leftSideLayout->update();

	editorLayout = new QVBoxLayout;
	editorLayout->setSpacing(4);
	editorLayout->addWidget(fileInfo);
	editorLayout->addWidget(textEditor);

	// TextEditor, RightBar
	QHBoxLayout* centralLayout = new QHBoxLayout;
	centralLayout->setSpacing(4);
	centralLayout->addItem(editorLayout);
	centralLayout->addItem(rightBarLayout);

	QHBoxLayout* bottomBarLayout = new QHBoxLayout;
	bottomBarLayout->addWidget(cmdLine);
	bottomBarLayout->addWidget(taskStatusLabel);

	QVBoxLayout* rightSideLayout = new QVBoxLayout;
	rightSideLayout->addLayout(centralLayout);
	rightSideLayout->addLayout(bottomBarLayout);

	// Main Layout (Combination of all child layouts)
	mainSplitter = new QSplitter(Qt::Horizontal);
	leftBar->setHidden(true);
	temp = new QWidget;
	leftBar->setLayout(leftSideLayout);
	temp->setLayout(rightSideLayout);
	mainSplitter->addWidget(leftBar);
	mainSplitter->addWidget(temp);
	mainSplitter->setStretchFactor(0,2);
	mainSplitter->setStretchFactor(1,5);

	QStackedLayout* stack = new QStackedLayout(this);
	stack->addWidget(mainSplitter);
	this->setLayout(stack);
}

void DGCentralWidget::setupConnections() {
	this->connect(mainSplitter,		SIGNAL(splitterMoved(int,int)),			 SLOT(resizeDirView()));
	this->connect(projectDirView,   SIGNAL(expanded(QModelIndex)),			  SLOT(resizeDirView()));
	this->connect(projectDirView,   SIGNAL(collapsed(QModelIndex)),			 SLOT(resizeDirView()));
	this->connect(projectComboBox,  SIGNAL(currentIndexChanged(int)),		   SLOT(changeProject(int)));
	this->connect(ctrl,			 SIGNAL(sigProjectListChanged()),			SLOT(updateProjectList()));
	this->connect(ctrl,			 SIGNAL(sigProjectClosed()),				 SLOT(shrinkProjectList()));
	this->connect(projectDirView,   SIGNAL(clicked(const QModelIndex&)),		SLOT(changeFile(const QModelIndex&)));

	connect(buttonsSide.at(DGCentralWidget::BUILD), SIGNAL(pressed()), ctrl, SLOT(build()));
	connect(buttonsSide.at(DGCentralWidget::REBUILD), SIGNAL(pressed()), ctrl, SLOT(rebuild()));
	connect(buttonsSide.at(DGCentralWidget::ABORT), SIGNAL(pressed()), ctrl, SLOT(abort()));
	connect(buttonsSide.at(DGCentralWidget::RUNFILE), SIGNAL(pressed()), ctrl, SLOT(runFile()));
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
	} else
		ctrl->getFile(ctrl->getPath());
	projectDirView->setHidden(!m);
	resizeDirView();
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
	leftBar->setHidden(hide);
}

