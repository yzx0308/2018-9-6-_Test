#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QPixmap>
#include <QImage>
#include <QBuffer>
#include <QPicture>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QtGlobal>
#include <QTextStream>
#include <QMutex>

#include "showpic.h"
#include "myvlc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    void readConfig();
    void initMenu();
    void initVlc();
private:
    Ui::MainWindow *ui;
    QMenu *mMenu;
    QAction *mAct;
    QList<QAction *> mActSet;
    QList<QString> mUrlSet;
    QMap<QAction *, QString> mXmlSet;

    ShowPic *mShowThrd;
    ShowPic *mShowThrdBro;
    QString mSavePath;
    quint32 mSaveHz;
    QStringList mStrListUrl;

//    void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

//    QActionGroup mActGroup;
private slots:
    void slotMenuTrig(QAction *);
    void slotSavePic(QString );
    void slotSavePicBro(QString );
};

#endif // MAINWINDOW_H
