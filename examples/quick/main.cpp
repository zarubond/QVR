#include <QGuiApplication>
#include <QQuickView>

#include "item.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Item>("Zarubond", 1, 0, "Item");

    QQuickView view;
    view.setSource(QUrl("qrc:/main.qml"));
    view.setResizeMode(QQuickView::SizeViewToRootObject);
    view.show();

    return app.exec();
}
