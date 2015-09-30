#ifndef LANGREFCOUNT_H
#define LANGREFCOUNT_H

#include <map>
#include <set>
#include <QString>

class QFileInfo;

/**
 * @brief Loads and reference counts language information.
 */
class LangRegistry {
	static const QString DIR;
	static const QRegExp FILEEXT_PATTERN;

	struct LangEntry {
		LangEntry() {refs = 0; buildsys = false;}
		size_t refs;
		bool buildsys;
		QString name;
	};
	struct FileEntry {
		explicit FileEntry(const QString& lang) {this->lang = lang;}
		QString lang;
		QString interpreter;
	};

	std::map<QString,FileEntry> fileexts;
	std::map<QString,FileEntry> filenames;
	std::map<QString,LangEntry> langs;

	inline std::map<QString,FileEntry>&        getBindMap      (bool isext)        {return isext?fileexts:filenames;}
	inline const std::map<QString,FileEntry>&  getBindMapConst (bool isext) const  {return isext?fileexts:filenames;}
public:
	static QString getFileExt(const QString& filename);

	LangRegistry();

	//Tests for known mappings.
	inline bool knowsFile(const QString& name, bool isext = true) const {return getBindMapConst(isext).count(name);}
	inline bool knowsLang(const QString& lang) const {return langs.count(lang);}

	/**
	 * @brief Gets a human-friendly untranslated language name.
	 * Returns a language name if specified in the language's properties.conf. Otherwise, defaults to the programmer name.
	 * This name is meant to be human-friendlier, and should be used in GUI elements or messages.
	 * It should also be passed through tr() first.
	 */
	const QString& getHumanName(const QString& lang) const;

	/**
	 * @brief Get the corresponding language for a file.
	 */
	const QString& getLang(const QFileInfo& file) const;

	/**
	 * @brief Get the corresponding language for a file extension or name
	 */
	const QString& getLang(const QString& name, bool isext = true) const; //Default true for teenagers.

	/**
	 * @brief Check if an interpreter exists for a given language.
	 */
	bool hasInterpreter(const QString& name, bool isext = true) const {return !getBindMapConst(isext).at(name).interpreter.isEmpty();}

	/**
	 * @brief Test if the registry has loaded a language for a certain file type.
	 */
	bool ready(const QString& name, bool isext = true) const;

	/**
	 * @brief Return whether or not a language describes files for a build system.
	 */
	bool isBuildSys(const QString& lang) const;

	/**
	 * @brief Gets the set of build systems that LangRegistry recognizes.
	 */
	std::set<QString> getBuildSysSet() const;

	/**
	 * @brief getInterpreter Get the interpreter for a certain language.
	 * @return A name or path to an executable that can interpret the file, or an empty string if none is known.
	 */
	QString getInterpreter(const QString& name, bool isext = true) const;

	//Reference counting, to be incremented per PROJECT, not PER FILE!
	bool add(const QStringList& langs);
	bool rem(const QStringList& langs);

	inline size_t countRefs(const QString& lang) const {return langs.at(lang).refs;}
	inline size_t countLanguages() const {return langs.size();}
	inline size_t countBindings() const {return fileexts.size() + filenames.size();}

};

#endif // LANGREFCOUNT_H
