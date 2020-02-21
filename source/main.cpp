#include <includes.h>
#include <dbmanager.h>
#include <firstwin.h>

int     main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(/*QCoreApplication::applicationDirPath() + */":icones/ressources/icons/app.png"));//app icon
	qDebug() << QCoreApplication::applicationDirPath();
	//translation Fr
	QString locale = QLocale::system().name().section('_', 0, 0);
	QTranslator translator;
	translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&translator);

	FirstWin Window;
	Window.set_path(/*QCoreApplication::applicationDirPath() + */"gestion.db");
	Window.show();
	return app.exec();
}
