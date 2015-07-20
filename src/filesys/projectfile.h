#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class QDir;

/**
 * @brief A class that stores, serializes, and deserializes project information.
 * Represents one instance of a traditional project file, namely .dgproject.conf
 * This file should only be used to tweak DevGarden's behavior. It shouldn't be required by end users of the project.
 */
class ProjectFile {
public:
	explicit ProjectFile(const QDir& dir);
	void serialize() const;
	ProjectFile* reload();
	const QJsonObject* data() const;
	const QJsonObject* data(const QJsonObject& set);
	QJsonValue& at(const QString& key) const;
	ProjectFile* get(const QString& key, QJsonValue** value) const;
	ProjectFile* set(const QString& key, QJsonValue* value = nullptr);
	bool isValid() const {return doc.isObject();}
	bool isEmpty() const {return doc.isEmpty() || !isValid();}
private:
	QJsonDocument doc;
};

#endif // PROJECTFILE_H
