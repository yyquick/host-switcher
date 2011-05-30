/*
 * HostConfig.cpp
 *
 *  Created on: 2011-4-28
 *      Author: yyquick
 */

#include "host_config.h"
#include <QByteArray>
#include <QRegExp>
#include <QStringList>
#include <QTextStream>
#include <iostream>
#ifdef Q_WS_WIN
#include <windows.h>
#include <tchar.h>
#endif

HostConfig::HostConfig() {
#ifdef Q_WS_WIN
	#define INFO_BUFFER_SIZE 1000
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	int len;
	if ( len = GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE)) {
		char buffer[1000];
		len = WideCharToMultiByte(CP_UTF8, NULL, infoBuf, len, buffer, 1000, NULL, NULL);
		buffer[len] = NULL;
		config_file_path_ = buffer;
		config_file_path_ = config_file_path_.append("/drivers/etc/hosts");
	} else {
		config_file_path_ = "C:/Windows/System32/drivers/etc/hosts";
	}
#else
	config_file_path_ = "/etc/hosts";
#endif
	parse_host_file();
}

HostConfig::~HostConfig() {
}

HostConfig::Section::Section(QString name, QString raw_content) {
	name_ = name;
	QTextStream raw_stream(&raw_content);
	QTextStream stream(&content_);
	QString line;
	is_enable_ = false;
	while (!raw_stream.atEnd()) {
		line = raw_stream.readLine().trimmed();
		if (!line.isEmpty()) {
			if (line[0] != '#') {
				is_enable_ = true;
			} else {
				line.remove(0, 1);
			}
			stream << line << endl;
		}
	}
	stream.flush();
}

HostConfig::Section::~Section() {

}

void HostConfig::parse_host_file() {
	QFile file;
	file.setFileName(config_file_path_);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	section_list_.clear();
	QByteArray content = file.readAll();
	QRegExp	first_rx("(.*)###### HostSwitcher Item: (.+) Start ######");
	first_rx.setMinimal(true);
	if (first_rx.indexIn(content.data(), 0) != -1) {
		QStringList match_list = first_rx.capturedTexts();
		Section first_sec("Common", match_list[1]);
		section_list_.append(first_sec);
	} else {
		Section first_sec("Common", content);
		section_list_.append(first_sec);
	}
	QRegExp	rx("###### HostSwitcher Item: (.+) Start ######(.+)###### HostSwitcher Item: .+ End ######");
	rx.setMinimal(true);

	int pos = 0;
	while ((pos = rx.indexIn(content.data(), pos)) != -1) {
		pos += rx.matchedLength();
		QStringList match_list = rx.capturedTexts();
		Section sec(match_list[1], match_list[2]);
		section_list_.append(sec);
	}
}

void HostConfig::save_info() {
	QFile file;
	file.setFileName(config_file_path_);
	if (!file.open(
			QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered))
		return;
	QTextStream write_stream(&file);
	HostConfig::SectionListIter iter;
	for (iter = section_list_.begin(); iter != section_list_.end(); iter++) {
		if (iter != section_list_.begin()) {
			write_stream << "###### HostSwitcher Item: " << iter->name_ << " Start ######" << endl;
		}
		QTextStream content_stream(&(iter->content_));
		while (!content_stream.atEnd()) {
			QString line;
			line = content_stream.readLine().trimmed();
			if (!line.isEmpty()) {
				if (!iter->is_enable_) {
					line = "#" + line;
				}
				write_stream << line << endl;
			}
		}
		if (iter != section_list_.begin()) {
			write_stream << "###### HostSwitcher Item: " << iter->name_ << " End ######" << endl;
		}
	}
}

void HostConfig::append_item(QString name, QString content) {
	Section sec(name, content);
	sec.is_enable_ = false;
	section_list_.append(sec);
}

void HostConfig::delete_item(int i) {
	section_list_.removeAt(i);
}
