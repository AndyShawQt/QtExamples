#include <QDebug>
#include <QQmlApplicationEngine>
#include <QAndroidService>
#include <QGuiApplication>
#include <QAndroidJniObject>
#include <QtAndroid>

int main(int argc, char *argv[])
{
    if (argc > 1) { // Service
        QAndroidService app(argc, argv);
        qDebug() << "I am the service";
        return app.exec();
    }
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(QStringLiteral("qrc:/"));
    engine.load(QUrl(QStringLiteral("qrc:/source/qml/main.qml")));
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/example/MyCustomAppService", "start",
                                              "(Landroid/content/Context;)V", QtAndroid::androidActivity().object());
    return app.exec();
}
