#include "host_switcher.h"

#include <QtGui>
#include <QApplication>
#include <QTextCodec>
#include <iostream>

#ifdef Q_OS_MACX
#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
#include <asl.h>
#include <unistd.h>
#include <pwd.h>
#endif

int main(int argc, char *argv[])
{
#ifdef Q_WS_WIN
	// set text codec
	QTextCodec *gbk = QTextCodec::codecForName("gb18030");
	QTextCodec::setCodecForTr(gbk);
	QTextCodec::setCodecForLocale(gbk);
	QTextCodec::setCodecForCStrings(gbk);

#else
	// set text codec
	QTextCodec *utg8 = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForTr(utg8);
	QTextCodec::setCodecForLocale(utg8);
	QTextCodec::setCodecForCStrings(utg8);
#endif

#ifdef Q_OS_MACX
	AuthorizationRef authRef;
	AuthorizationCreate(NULL, NULL, 0, &authRef);
	AuthorizationRightSet(authRef, "com.host-switcher.hosts.write", CFSTR(kAuthorizationRuleIsAdmin), CFSTR("modify hosts file"), NULL, NULL);

	AuthorizationRights rights, *gotRights;
	AuthorizationItem authItem[1];
	authItem[0].name = "com.host-switcher.hosts.write";
	authItem[0].valueLength = 0;
	authItem[0].value = NULL;
	authItem[0].flags = 0;
	rights.count = 1;
	rights.items = authItem;

	OSStatus status = AuthorizationCopyRights(authRef, &rights, kAuthorizationEmptyEnvironment, kAuthorizationFlagExtendRights | kAuthorizationFlagInteractionAllowed, &gotRights);
	if (status != errAuthorizationSuccess) {
		exit(1);
	}

	struct passwd *pwdInfo = getpwuid(getuid());
	QString cmd = pwdInfo->pw_name;
	cmd += " allow write";
	AuthorizationFlags flags = kAuthorizationFlagDefaults;
	const char * arguments[] = {"+a", cmd.toAscii().constData(), "/etc/hosts", NULL};
	status = AuthorizationExecuteWithPrivileges(authRef, "/bin/chmod", flags, (char **)arguments, NULL);
	if (status != errAuthorizationSuccess) {
		exit(1);
	}

#endif
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    HostSwitcher w;
    w.show();
    int ret = a.exec();

#ifdef Q_OS_MACX
	AuthorizationFree(authRef, kAuthorizationFlagDefaults);
#endif
    return ret;
}
