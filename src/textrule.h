#ifndef TEXTRULE_H
#define TEXTRULE_H

#include <QObject>
#include <QRegExp>

#include <vector>

struct MatchList {
	std::vector<std::pair<size_t,size_t>> matches;
};

/**
 * @brief Caching text matcher. Remember kids, don't play with matches.
 */
class TextMatcher : public QObject {
	Q_OBJECT
public:
	TextMatcher() {
		dirty = false;
		handledeletion = true;
		tomatch = nullptr;
		matches = nullptr;
	}

	virtual ~TextMatcher() {for(QRegExp* rule : rules) delete rule;}

	TextMatcher* attach(const QString* str) {if(tomatch != str) {markDirty(); tomatch=str;} return this;}
	const MatchList* match(const QString* str = nullptr) {if(str) doMatching(str); else doMatching(tomatch); return matches;}
	const MatchList* getMatches() {return (dirty?nullptr:matches);}
	MatchList* disownMatches() {handledeletion = false; return matches;}

public slots:
	void markDirty() {dirty = true;}
protected:
	void doMatching(const QString* matches);
	std::vector<QRegExp*> rules;
	//std::vector<TextRule*> contexts; //No ownership.
	//std::vector<TextRule*> excludes; //No ownership.
private:
	const QString* tomatch; //No ownership.
	MatchList* matches;
	bool dirty, handledeletion;
};

#endif // TEXTRULE_H
