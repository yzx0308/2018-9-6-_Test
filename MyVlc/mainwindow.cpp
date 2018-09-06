#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->labelVideo->setVisible(true);
    setWindowTitle(tr("轴承视频"));
    readConfig();
    initMenu();

    setWindowState(Qt::WindowMaximized);

//    setWindowFlag(Qt::WindowState::WindowMaximized);
    mMenu = new QMenu( tr("视频"), this);
    ui->menuBar->addMenu(mMenu);
    mMenu->addActions(mActSet);

    mShowThrd = new ShowPic(this, mSaveHz );
    mShowThrdBro = new ShowPic(this, mSaveHz);

    initVlc();

    connect(ui->menuBar,SIGNAL(triggered(QAction *)),this, SLOT(slotMenuTrig(QAction *)));

    connect(mShowThrd, SIGNAL(emtSavePic(QString)), this, SLOT(slotSavePic(QString)));
    connect(mShowThrdBro, SIGNAL(emtSavePic(QString)), this, SLOT(slotSavePicBro(QString)));

    mShowThrd->start();
    mShowThrdBro->start();

    qDebug() << "============";
    qDebug() << this->thread();
    qDebug() << mShowThrd->thread();
    qDebug() << mShowThrdBro->thread();
    qDebug() << "============";
}

MainWindow::~MainWindow()
{
    mShowThrd->terminate();
    mShowThrdBro->terminate();
    delete ui;
}

void MainWindow::readConfig()
{

    int curCount;
    int urlCount;

    QDomElement e;
    QDomDocument doc;
    QFile file("config.xml");
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode node = docElem.firstChild();

//    qDebug() << node.toElement().tagName() << ":" << node.toElement().text();

    if(node.toElement().tagName() != "urlNumber")
    {
        qDebug() << "errer xml format.";
        return;
    }

    urlCount = node.toElement().text().toInt(NULL, 10);
    qDebug() << urlCount;

    node = node.nextSibling();

    while(!node.isNull())
    {
        curCount++;
        if(curCount <= urlCount)
        {
            e = node.toElement(); // try to convert the node to an element.
            if(!e.isNull())
            {
                qDebug()<< "top:" << e.tagName() << ":" << e.text().toLocal8Bit();

                QAction *act = new QAction(e.text(), this);
                if(mActSet.count() < urlCount)
                    mActSet.push_back(act);

//                ===========waringggggggggggggg=============
                if(e.tagName() == tr("savePath"))
                    mSavePath = e.text().toLocal8Bit();
                else if( e.tagName() == tr("saveHz"))
                {
                    qDebug() << "in Hz：" << 1;
                    mSaveHz = e.text().toInt();
                    break;
                }
//                ===========waringggggggggggggg=============

                node = node.nextSibling();  //获取url
                e = node.toElement();


//                mUrlSet.push_back(node.toElement().text());

//                ===========waringggggggggggggg=============
//                导致inivlc的调用必须在readConfig之后
//                QString strTemp = node.toElement().text();
//                QString *str = const_cast<QString *>(&strTemp);
//                qDebug() << "path：" << strTemp;
//                mShowThrd->url = *str;
//                ===========waringggggggggggggg=============

                if(mXmlSet.count() < urlCount)
                {
                    mXmlSet.insert(act, e.text());
                    mStrListUrl.push_back(e.text());
                }
            }

            node = node.nextSibling();   //获取name
        }

    }

    if(mSaveHz < 1 || mSaveHz >24)
        mSaveHz = 1;
    qDebug() << "saveHz:" << mSaveHz;

    qDebug("=======================================");
    foreach (QString temp, mStrListUrl) {
        qDebug() << temp;
    }
    qDebug("=======================================");

    QString title;
    if(ui->stackedWidget->currentIndex() == 0)
        title = mXmlSet.keys().at(0)->text();
    else if(ui->stackedWidget->currentIndex() == 1)
        title = mXmlSet.keys().at(1)->text();
    title.prepend(tr("轴承视频/"));
//    setWindowTitle(title);

}

void MainWindow::initMenu()
{

}

void MainWindow::initVlc()
{
//    url = "rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov";
    mShowThrd->threadNum = 0;
    mShowThrd->mSavePath = mSavePath;
    if(mStrListUrl.size() > 0)
        mShowThrd->url = mStrListUrl.at(0);
    else
        mShowThrd->url = "";

    mShowThrd->vlc->setMedia(mShowThrd->url.toLatin1().data(), ui->labelVideo->winId());
    mShowThrd->vlc->setCache(200);

    mShowThrdBro->threadNum = 1;
    mShowThrdBro->mSavePath = mSavePath;
    if(mStrListUrl.size() > 0)
        mShowThrdBro->url = mStrListUrl.at(1);
    else
        mShowThrd->url = "";
    mShowThrdBro->vlc->setMedia(mShowThrdBro->url.toLatin1().data(), ui->labelVideo_2->winId());
    mShowThrdBro->vlc->setCache(200);
}



void MainWindow::slotMenuTrig(QAction *a)
{
#if 0
    QList<QAction *> valList = mXmlSet.keys();

    if(a->text() == valList.at(0)->text())
    {
        ui->labelVideo_2->hide();
        ui->stackedWidget->setCurrentIndex(0);

        if(!ui->labelVideo->isVisible())
            ui->labelVideo->setVisible(true);
    }
    else if(a->text() == valList.at(1)->text())
    {
        ui->labelVideo->hide();
        ui->stackedWidget->setCurrentIndex(1);

        if(!ui->labelVideo_2->isVisible())
            ui->labelVideo_2->setVisible(true);
    }

    QString str = tr("轴承视频");
    str += "/" + a->text();
    setWindowTitle(str);
#endif
}

void MainWindow::slotSavePic(QString url)
{
    qDebug() << "stream 1 : " << QDateTime::currentDateTime().toString("yy-MM-dd_hh-mm-ss");
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
           /* qDebug() << "create path true:" << curPath*/;
        }
        else
        {
           /* qDebug() << "create path false:" << curPath*/;
        }
    }
#if 0
    if(url.contains("stream1"))
        fileName.prepend("stream1");
    else
        fileName.prepend("stream2_");
#endif
    fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    fileName += QString(tr("_%1").arg(mShowThrd->threadNum));
    fileName += tr(".png");
    fileName.prepend("stream1_");
    curPath.append("/");
    curPath.append(fileName);
   // qDebug() << "save path stream1:" << curPath;
    if(mShowThrd->vlc->snapShot(curPath.toLatin1().constData(), 0, 0) == 0)
    {
        qDebug() << "save success...";

    }
    else
        qDebug() << "sava failed...";

}

void MainWindow::slotSavePicBro(QString url)
{

//    qDebug() << "stream 2 : " << QDateTime::currentDateTime().toString("yy-MM-dd_hh-mm-ss");
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
    fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    fileName += QString(tr("_%1").arg(mShowThrdBro->threadNum));
    fileName += tr(".png");
    fileName.prepend("stream2_");
    curPath.append("/");
    curPath.append(fileName);
//    qDebug() << "save path stream2:" << curPath;
    if(mShowThrd->vlc->snapShot(curPath.toLatin1().constData(), 0, 0) == 0)
    {
        qDebug() << "save success...";
//        QFile
    }
    else
        qDebug() << "sava failed...";
}
