#ifndef LANGREFCOUNT_H
#define LANGREFCOUNT_H

#include <map>
#include <QString>

/**
 * @brief Loads and reference counts language information.
 */
class LangRegistry {
	static const QString DIR;
	static const QString EMPTY;
	struct LangEntry {
		LangEntry() {refs = 0;}
		size_t refs;
		QString default_interpreter;
	};
	struct ExtEntry {
		explicit ExtEntry(const QString& lang) {this->lang = lang;}
		QString lang;
		QString interpreter;
	};

	std::map<QString,ExtEntry> fileexts;
	std::map<QString,LangEntry> langs;
public:
	LangRegistry();

	//Tests for known mappings.
	inline bool knowsExt(const QString& fileext) const {return fileexts.count(fileext);}
	inline bool knowsLang(const QString& lang) const {return langs.count(lang);}

	/**
	 * @brief Get the corresponding language for a file extension
	 */
	const QString& getLang(const QString& fileext) const;

	/**
	 * @brief Check if an interpreter exists for a given language.
	 */
	inline bool hasInterpreter(const QString& lang) const {return !langs.at(lang).default_interpreter.isEmpty();}

	/**
	 * @brief Test if the registry has loaded a language for a certain file extension.
	 */
	bool ready(const QString& fileext) const;

	/**
	 * @brief getInterpreter Get the interpreter for a certain language.\
	 * @return A name or path to an executable that can interpret the file, or an empty string if none is known.
	 */
	QString getInterpreter(const QString& lang) const;

	/**
	 * Loads a language by file extension, increments the reference count for it, and returns the language name.
	 * To be used when a project needs to open a file of an unexpected language.
	 */
	const QString& load(const QString& fileext);

	//Reference counting, to be incremented per PROJECT, not PER FILE!
	bool add(const QStringList& langs);
	bool rem(const QStringList& langs);

	inline size_t countRefs(const QString& lang) const {return langs.at(lang).refs;}
	inline size_t countLanguages() const {return langs.size();}
	inline size_t countFileexts() const {return fileexts.size();}

};

#endif // LANGREFCOUNT_H
