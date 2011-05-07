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

class HostConfig {
public:
	HostConfig();
	virtual ~HostConfig();
	void parse_host_file();
	void save_info();
	void append_item(QString name, QString content);
	void delete_item(int i);

	struct Section {
	public:
		QString name_;
		QString content_;
		bool is_enable_;

		Section(QString name, QString raw_content);
		~Section();
	};

public:
	QList< Section > section_list_;
	typedef QList< Section >::iterator SectionListIter;

protected:
	QString config_file_path_;

};

#endif /* HOSTCONFIG_H_ */
