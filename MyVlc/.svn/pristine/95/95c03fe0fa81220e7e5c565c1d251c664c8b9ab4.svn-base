#include "showpic.h"
#include "qdebug.h"

ShowPic::ShowPic(QObject *parent, quint32 Hz) :
    QThread(parent),runFlag(true), isPlay(false),
    isStop(false), mTimerFlag(false), snapCount(0)
{
    /* init vlc */

    saveHz = Hz;
    vlc = new MyVlc();

    mTimer = new QTimer();
    mCpTimer = new QTimer();
    connect(mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(mCpTimer,SIGNAL(timeout()),this, SLOT(onCpTimeout()));
    mTimer->start(1000 * 60 * 20);

}

ShowPic::~ShowPic()
{
    vlc->stop();
    this->stop();
    isPlay =false;
    if(vlc != NULL)
        delete vlc;
}

void ShowPic::run()
{

//    while (true)
//    {
        if(isStop)
        {
           return;
        }

        if(!isPlay)
        {
            if(vlc->play() < 0)
            {
                qDebug() << "error...\n";
            }
            else
            {
                qDebug() << "*********ok : " << this->currentThread();
                mStartTime = QDateTime::currentDateTime().time();
                emit emtSavePic(url);
                isPlay = true;
            }
        }
        msleep(1000);
/*
 * QObject是可重入的，它的大多数非GUI子类，例如
 * QTimer, QTcpSocket, QUdpSocket and QProcess
 * 都是可重入的，使得这些类可以同时用于多线程。
 * 需要注意的是，这些类设计为从一个单一的线程创建和使用的，
 * 在一个线程创建对象，而从另外一个线程调用对象的函数并不能保证行得通。
*/
//        mTimer = new QTimer();
//        connect(mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
//        mTimer->start(1000);


//        mTimer->setSingleShot(false);


#if 0
        mCurTime = QDateTime::currentDateTime().time();


//        int varSec = abs();
//        bool flag = varSec == 30 ? true : false;



//        if(mCurTime.minute() == mStartTime.minute() && mCurTime.second() == (mStartTime.second()))

//        {
//            emit emtSavePic();
//        }
        int varSec = qAbs(mCurTime.second() - mStartTime.second());
        if(varSec % 5 == 0)
        {
            qDebug() << "emit emtSavePic...";
            emit emtSavePic(url);
        }

#else
        ;
#endif
//    }
}

void ShowPic::stop()
{
    isStop = true;
    this->quit();
    this->wait();
}

void ShowPic::onTimeout()
{
    mCpTimer->start(1000);

}

void ShowPic::onCpTimeout()
{
//    qDebug("========time out=========");
#if 1
//    emit emtSavePic(url);

    QString fileName;
    QString curPath;
    QDir saveDir;

    if(mSavePath.isEmpty())
        mSavePath = QDir::currentPath();

    curPath = mSavePath;
    curPath += "/image/";
    curPath += QDateTime::currentDateTime().toString("yyyyMMdd");
    saveDir.setPath(curPath);

    if( !saveDir.exists() )
    {
        if(saveDir.mkpath(curPath))
        {
            /*qDebug() << "create path true:" << curPath*/;
        }
        else
        {
            /*qDebug() << "create path false:" << curPath*/;
        }
    }
#if 0
    if(url.contains("stream1"))
        fileName.prepend("stream1");
    else
        fileName.prepend("stream2_");
#endif
   // for(int i = 0; i < 10; i++)
   // {
        fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
//        fileName.append("_%1").arg(i);
        fileName += tr(".png");
        fileName.prepend(tr("stream%1_").arg(this->threadNum));
        curPath.append("/");
        curPath.append(fileName);
        qDebug() << tr("save path stream%1:").arg(threadNum) << curPath;
        if(vlc->snapShot(curPath.toLatin1().constData(), 0, 0) == 0)
        {
            qDebug() << "save success...";
        }
        else
            qDebug() << "sava failed...";

        qDebug() << "###################";

        snapCount++;
        if(snapCount == 10)
        {
            snapCount = 0;
            mCpTimer->stop();
        }

//        if(!mTimerFlag)
//        {
//            msleep(1000);
//            snapCount++;
//            if(snapCount == 10)
//            {
//                mTimerFlag = true;
//                snapCount = 0;
//            }
//        }
//        else
//        {
//            msleep(1000 * 60 * 20);
//            mTimerFlag = false;
//        }
   // }


#else

//    threadNum
    QString curPath = QDateTime::currentDateTime().time().toString("hh_mm_ss");
    curPath.append(QString(tr("_%1").arg(this->threadNum)));
//    QDateTime::currentDateTime().toString()
    curPath += ".png";

    if(vlc->snapShot(curPath.toLatin1().constData(), 0, 0) == 0)
    {
        qDebug() << this->thread() << "save success...";
    }
    else
        qDebug() << this->thread() << "sava failed...";
#endif
}

int ShowPic::init()
{

}
