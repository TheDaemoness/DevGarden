#include "buildcontroller.h"

#include "filesys/dgprojectloader.h"

BuildController::BuildController(DGProjectLoader& pl, QObject *parent) : QObject(parent) {
	this->pl = &pl;
}

void BuildController::currentBuild() {
}

void BuildController::currentRebuild() {

}

