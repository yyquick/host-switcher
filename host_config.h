/*
 * HostConfig.h
 *
 *  Created on: 2011-4-28
 *      Author: yyquick
 */

#ifndef HOSTCONFIG_H_
#define HOSTCONFIG_H_

#include <QString>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <QStringList>
#include <QMap>

class HostConfig {
public:
	HostConfig();
	virtual ~HostConfig();
	void parse_host_file();
	void save_info();
	void append_item(QString name, QString content);
	void delete_item(int i);
	void import_config_content(QString url, QString content);

	struct Section {
	public:
		QString name_;
		QString content_;
		bool is_enable_;
		bool share_;

		Section(QString name, QString raw_content);
		~Section();

		void merge(QString content);
	};

public:
	QList< Section > section_list_;
	QMap< QString, QString > config_;
	typedef QMap< QString, QString >::iterator ConfigIter;
	typedef QList< Section >::iterator SectionListIter;

	static QString HOTKEY_MOVE_UP_KEY;
	static QString HOTKEY_MOVE_DOWN_KEY;
	static QString HOTKEY_RESTORE_KEY;

	void merge(QString name, QString content);
	void set(QString name, QString content);
	QString getShareData();
	bool hasShareData();

protected:
	QString config_file_path_;
	int find(QString name);

};

#endif /* HOSTCONFIG_H_ */
