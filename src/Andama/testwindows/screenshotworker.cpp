#include "screenshotworker.h"
#include "qdebug.h"

#ifdef WIN32
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#else
#include "thread"
#include "chrono"
#endif

screenshotWorker::screenshotWorker()
{
}

void screenshotWorker::Starts(QLabel *g, cs_thread *cst)
{

    originalPixmap = QPixmap(); // clear image for low memory situations
                                     // on embedded devices.

   while(true){
   originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    originalPixmap.save(&buffer,"JPG",1); // writes pixmap into bytes in PNG format

    //qDebug("buffer length: %i",bytes.count());

    //const char *data =bytes.constData();

    int nSize = bytes.size();
     qDebug("bytes size: %i",nSize);

    char * cc = new char[nSize]; //char cc[nSize];
    bzero(cc,nSize);
    //memcpy(cc,bytes.constData(),nSize);
    for(int i=0;i<nSize;i++){
        //qDebug("create: %i, byte: %i",i,bytes.at(i));
        cc[i]=bytes.at(i);
    }

    qDebug("bytes size: %i",bytes.size());
    qDebug("cc sizeof: %i",sizeof(cc));

    char CMD_SCREENSHOT[] = "S";

    cst->clsrv.sendmsg2(cst->clsrv.localsocket,CMD_SCREENSHOT,cc,bytes.size());
//std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //sleep_for(std::chrono::milliseconds(200));

//QPixmap fromb;
    //fromb.loadFromData(bytes);
    //g->setPixmap(fromb.scaledToHeight(g->height()));
}

}
