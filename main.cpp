#include "host_switcher.h"

#include <QtGui>
#include <QApplication>
#include <QTextCodec>
#include <iostream>

//#ifdef Q_OS_MACX
//#include <CoreFoundation/CoreFoundation.h>
//#include <Security/Security.h>
//#include <asl.h>
//#include <unistd.h>
//#endif

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

//#ifdef Q_OS_MACX
//	AuthorizationRef authRef;
//	AuthorizationCreate(NULL, NULL, 0, &authRef);
//	AuthorizationRightSet(authRef, "com.host-switcher.hosts.write", CFSTR(kAuthorizationRuleIsAdmin), CFSTR("modify hosts file"), NULL, NULL);

//	AuthorizationRights rights, *gotRights;
//	AuthorizationItem authItem[1];
//	authItem[0].name = "com.host-switcher.hosts.write";
//	authItem[0].valueLength = 0;
//	authItem[0].value = NULL;
//	authItem[0].flags = 0;
//	rights.count = 1;
//	rights.items = authItem;

//	OSStatus osst = AuthorizationCopyRights(authRef, &rights, kAuthorizationEmptyEnvironment, kAuthorizationFlagExtendRights | kAuthorizationFlagInteractionAllowed, &gotRights);
//	std::cout << (*gotRights).items[0].name << std::endl;

//	int retSetuid = setuid(0);
//	std::cout << retSetuid << std::endl;
//#endif
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
