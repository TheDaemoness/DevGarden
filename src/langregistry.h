#ifndef LANGREFCOUNT_H
#define LANGREFCOUNT_H

#include <vector>
#include <map>
#include <set>
#include <QString>

class QFileInfo;

/**
 * @brief Loads and reference counts language information.
 */
class LangRegistry {
	static const QString DIR;
public:
	struct LangEntry {
        LangEntry() : refs{0}, buildsys(""), name("") {}
		size_t refs;
		QString buildsys;
		QString name;
	};
	struct FileEntry {
		LangEntry* lang;
		QString interpreter;
        bool operator<(const FileEntry& b) const {return lang<b.lang || interpreter<b.interpreter;} //WARNING: Voodoo.
        FileEntry(LangEntry* language, const QString& interp = "") : lang{language}, interpreter(interp) {}
	};
    using LangID = size_t;

    static constexpr LangID ID_UNKNOWN = static_cast<LangID>(-1);

private:
    using FileMap = std::map<QString,const FileEntry*>;

    std::set<FileEntry>    cache_fileentries;
    std::vector<LangEntry> cache_langentries;

	FileMap fileexts;
	FileMap filenames;
    std::map<QString,LangEntry*> langs;

	inline       FileMap& getBindMap     (bool isext)       {return isext?fileexts:filenames;}
	inline const FileMap& getBindMapConst(bool isext) const {return isext?fileexts:filenames;}
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
	QString getHumanName(LangID id) const;

	/**
	 * @brief Get the corresponding language for a file.
	 */
	LangID getLang(const QFileInfo& file) const;

	/**
	 * @brief Get the corresponding language for a file extension or name
	 */
	LangID getLang(const QString& name, bool isext = true) const; //Default true for teenagers.

	/**
	 * @brief Check if an interpreter exists for a given language.
	 */
	bool hasInterpreter(const QString& name, bool isext = true) const;
	bool hasInterpreter(const QFileInfo& fi) const;

	/**
	 * @brief Test if the registry has loaded a language for a certain file type.
	 */
	bool ready(const QString& name, bool isext = true) const;

	/**
	 * @brief Return whether or not a language describes files for a build system.
	 */
	bool isBuildSys(LangID id) const;

	/**
	 * @brief Get the name build system invocation script in %tools/build/<lang>
	 */
	QString getBuildSys(LangID id) const;

	/**
	 * @brief Gets the set of build systems that LangRegistry recognizes.
	 */
	std::set<LangID> makeBuildSysSet() const;

	/**
	 * @brief getInterpreter Get the interpreter for a certain language.
	 * @return A name or path to an executable that can interpret the file, or an empty string if none is known.
	 */
	QString getInterpreter(const QString& name, bool isext = true) const;
	QString getInterpreter(const QFileInfo& fi) const;

	//Reference counting, to be incremented per PROJECT, not PER FILE!
	bool add(const QStringList& langs);
	bool rem(const QStringList& langs);

	inline size_t countRefs(LangID lang) const {return cache_langentries.at(lang).refs;}
	inline size_t countLanguages() const {return langs.size();}
	inline size_t countBindings() const {return fileexts.size() + filenames.size();}

};

#endif // LANGREFCOUNT_H
