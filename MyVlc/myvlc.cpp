#include "myvlc.h"
#include <QDebug>

MyVlc::MyVlc(QObject *parent) : QObject(parent)
{
    //不显示字幕和截图预览,可参考VLC的启动参数设置更多的参数.
    char* argv[2]={"--no-osd","--no-snapshot-preview"};
    base = libvlc_new(2,argv);;
    media = NULL;
    player = libvlc_media_player_new(base);
    cache = 2000;

}

MyVlc::~MyVlc()
{
    libvlc_media_player_release(player);
    libvlc_media_release(media);
    libvlc_release(base);
}

void MyVlc::setCache(int cache)
{
    this->cache = cache;
}

void MyVlc::setMedia(char *address, WId id)
{

    libvlc_media_release(media);
    media = libvlc_media_new_location(base, address);
//    media = libvlc_media_new_location(base, "screen://");

    libvlc_media_player_set_media(player, media);
    if (id == 0)
        libvlc_media_player_set_hwnd(player, NULL);
    else
        libvlc_media_player_set_hwnd(player, (void *)id);

}

void MyVlc::setId(WId id)
{
    libvlc_media_player_set_hwnd(player, (void *)id);
}

void MyVlc::setVolume(int volume)
{
    libvlc_audio_set_volume(player, volume);
}

int MyVlc::getVolume()
{
    libvlc_audio_get_volume(player);
}

char *MyVlc::getUrl()
{
    return libvlc_media_get_mrl(media);
}

int MyVlc::snapShot( const char * filePath, int width, int height)
{
    int ret = libvlc_video_take_snapshot(player, 0, filePath, width, height);

    return ret;
}

int MyVlc::play()
{
    //获得char ×类型的缓冲时间参数。
    QString para = "network-caching=" + QString::number(cache);
    QByteArray ba = para.toLatin1();
    char *add = ba.data();
    libvlc_media_add_option(media, add);
    int a = libvlc_media_player_play(player);

    return a;

}


void MyVlc::pause()
{
    libvlc_media_player_pause(player);
}

void MyVlc::stop()
{
    libvlc_media_player_stop(player);
}

int MyVlc::showPlay()
{
//   sleep(3);
    return this->play();
}
