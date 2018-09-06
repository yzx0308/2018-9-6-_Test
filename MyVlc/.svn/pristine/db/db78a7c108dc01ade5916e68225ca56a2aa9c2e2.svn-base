#ifndef SHOWPIC_H
#define SHOWPIC_H


#include <QThread>
#include <QImage>
#include <QString>
#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <QDir>

#include "myvlc.h"


class ShowPic : public QThread
{
    Q_OBJECT
public:
    explicit ShowPic(QObject *parent = 0, quint32 Hz = 0);
    ~ShowPic();

    int init();
    QString url;

    MyVlc *vlc;
    bool isPlay;
    bool isStop;
    quint32 saveHz;

    int threadNum;
    QString mSavePath;
protected:
    void run();
    void stop();

private:
    bool runFlag;
    QTime mStartTime;
    QTime mCurTime;

    QTimer *mTimer;
    QTimer *mCpTimer;
    bool mTimerFlag;
    int snapCount;


signals:
    void emtSavePic(QString );

public slots:
    void onTimeout();
    void onCpTimeout();
};

#endif // SHOWPIC_H
