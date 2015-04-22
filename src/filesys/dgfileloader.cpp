#include "dgfileloader.h"

#include <QString>

DGFileLoader::DGFileLoader(size_t save_buffer) {
	save_buffer_len = save_buffer;
	current.second = nullptr;
}

void DGFileLoader::fileEdited() {
	if(isCurrentSaved()) {
		unsaved_files.push_back(saved_files.front());
		saved_files.pop_front();
	}
}

void DGFileLoader::closeCurrent() {
	if(isCurrentSaved())
		saved_files.pop_front();
	else
		unsaved_files.pop_front();
	delete current.second;
	current.second = nullptr;
	current.first.clear();
}

void DGFileLoader::closeOthers(bool do_save) {
	bool saved = isCurrentSaved();
	if(saved)
		saved_files.pop_front();
	else
		unsaved_files.pop_front();
	for(auto e : saved_files)
		delete e.second;
	for(auto e : unsaved_files) {
		if(do_save)
			save(e.second, e.first);
		delete e.second;
	}
	saved_files.clear();
	unsaved_files.clear();
	if(saved)
		saved_files.push_back(current);
	else
		unsaved_files.push_back(current);
}

void DGFileLoader::closeSaved() {
	bool saved = isCurrentSaved();
	if(saved)
		saved_files.pop_front();
	for(auto e : saved_files)
		delete e.second;
	saved_files.clear();
	if(saved)
		saved_files.push_back(current);
}

void DGFileLoader::saveCurrent() {
	if(!isCurrentSaved()) {
		save(current.second,unsaved_files.front().first);
		unsaved_files.pop_front();
		appendSaved(current);
	}
}

void DGFileLoader::unloadSaved() {
	delete saved_files.back().second;
	saved_files.pop_back();
}

void DGFileLoader::appendSaved(const std::pair<QString,QTextDocument*>& doc) {
	saved_files.push_front(doc);
	if(saved_files.size() > save_buffer_len)
		unloadSaved();
}

void DGFileLoader::saveCurrentAs(const QString& path) {
	save(current.second,path);
}

void DGFileLoader::saveAll() {
	bool saved = isCurrentSaved();
	if(saved)
		saved_files.pop_front();
	auto i = unsaved_files.rbegin();
	auto e = unsaved_files.rend();
	for(;i!=e;++i) {
		save(i->second,i->first);
		appendSaved(*i);
	}
	saved_files.clear();
	if(saved)
		saved_files.push_back(current);
}

void DGFileLoader::save(QTextDocument *tosave, const QString &path) {
	//TODO: Implement.
}

QTextDocument* get(const QString& fi, bool enqueue = true) {

}
