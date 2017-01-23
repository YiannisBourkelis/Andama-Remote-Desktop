/* ***********************************************************************
 * Andama
 * (C) 2014 by Yiannis Bourkelis (hello@andama.org)
 *
 * This file is part of Andama.
 *
 * Andama is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Andama is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Andama.  If not, see <http://www.gnu.org/licenses/>.
 * ***********************************************************************/

#ifdef WIN32
#define NOMINMAX
#endif

#include "screenshotsworker.h"
#include "../Shared/AndamaHeaders/byte_functions.h"
#include "../Shared/AndamaHeaders/socket_functions.h"

screenshotsWorker::screenshotsWorker()
{
    isDirty = false;
}

void screenshotsWorker::run(void){
    while (true && !stopThread)
    {
        if (isDirty.load() == true)
        {
            //qDebug("9. screenshotsWorker: isDirty.load() == true. pendingScreenshot_mutex.lock()");
            { // lock_guard scope
                std::lock_guard<std::mutex> lock(pendingScreenshot_mutex);
                isDirty.store(false);
                pendingScreenshot_working_copy = pendingScreenshot.copy();
            } // lock_guard scope
            //qDebug("10. screenshotsWorker: isDirty set to false. pendingScreenshot_mutex.unlock()");

            if (this->pendingMsg.load() == this->protocol_supervisor->protocol.MSG_SCREENSHOT_DIFF_REQUEST){
                this->prepareAndSendScreenshotDiff();
            }
            else
            {
                //prwto olokliro screenshot
                previousScreenshot=pendingScreenshot_working_copy;
                this->sendScreenshot(this->pendingScreenshot_working_copy,-1,-1);
            }
        }

        QThread::currentThread()->msleep(1);
    }

    std::cout << "screenshotsWorker::run exiting...\r\n" << std::endl;
}

void screenshotsWorker::setScreenshot(const QImage &newScreenshot, const int msg)
{
    //QMutexLocker locker(&pendingScreenshot_mutex);

    //qDebug("7. Inside screenshotsWorker. Will set pendingScreenshot. pendingScreenshot_mutex.lock()");
    { // lock_guard scope
        std::lock_guard<std::mutex> lock(pendingScreenshot_mutex);
        pendingScreenshot = std::move(newScreenshot); //newScreenshot.copy();
        pendingMsg.store(msg);
        isDirty.store(true);
    } // lock_guard scope
   //qDebug("8. Inside screenshotsWorker. pendingScreenshot set. pendingScreenshot_mutex.unlock()");
}

void screenshotsWorker::prepareAndSendScreenshotDiff()
{
   //qDebug("11. screenshotsWorker.prepareAndSendScreenshotDiff called. Sygrisi me proigoumeno screenshot.");

    //dimiourgw antigrafo tou pending screenshot wste na klithei kai pali
    //std::cout << "prepareAndSendScreenshotDiff : Enarksi elegxou screenshot kai apostolis" << std::endl;

    QImage outimg;

       //sigrinw to proigoumeno screenshot kai to trexon gia diafores

       //TODO: algorithmos sygrisis ginetai arketa pio grigoros an
       //anti na sygrinei to kathe pizel, sigrinei ana dyo pixel, toso orizontia oso kai katheta.
       //An den kanw lathos to ofelos stin taxytita  einai x4. Se debub metrisa: 28ms me elegxo ana 1px kai 7ms me elegxo ana +=2
       //Episis tha mporousa na temaxisw tin othoni se orthogwnia tmimata, wste na stelnontai mikroteres eikones otan p.x. allazei kati
       //mikro panw aristera sto desktop kai katw deksia.
       //Vevaia den pianei oles tis allages sto desktop, alla einai anepesthiti i diafora.
       //Gia kalytera apotelesmata tha mporousa isws to x na to pigainw xiasta, diladi mia na ksekina apo 0 mia apo 1.
       //Episis sto telos, meta tis loupes, mporw na afairw/prosthetw analogos apo 1px sta minx,miny klp wste
       //na einai pixel perfect afto pou stelnei.
        int minx=pendingScreenshot_working_copy.width()+1;
        int maxx=-1;
        int miny=pendingScreenshot_working_copy.height()+1;
        int maxy=-1;
        int compareOffset = 4;
        //auto start = std::chrono::steady_clock::now();
        for (int ix=0;ix<pendingScreenshot_working_copy.width();ix+=compareOffset)
        {
             for (int iy=0;iy<pendingScreenshot_working_copy.height();iy+=compareOffset)
             {
                 if (previousScreenshot.pixel(ix,iy) != pendingScreenshot_working_copy.pixel(ix,iy)){
                     //yparxei diafora sta pixels
                     minx=std::min(minx,ix);
                     miny=std::min(miny,iy);
                     maxx=std::max(maxx,ix);
                     maxy=std::max(maxy,iy);
                     previousScreenshot.setPixel(ix,iy,pendingScreenshot_working_copy.pixel(ix,iy));
                 }
             }
        }

        /*
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << std::chrono::duration<double,std::milli>(diff).count() << " ms" <<std::endl;
        */

        //efoson to proigoumeno image kai to trexon den einai idia
        //stelnw to img tis diaforas tous
        if (maxx>-1)
        {
            minx = minx - compareOffset + 1 < 0 ? 0 : minx - compareOffset + 1;
            miny = miny - compareOffset + 1 < 0 ? 0 : miny - compareOffset + 1;
            maxx = maxx + compareOffset - 1 > pendingScreenshot_working_copy.width() - 1 ? pendingScreenshot_working_copy.width() - 1 :maxx + compareOffset - 1;
            maxy = maxy + compareOffset - 1 > pendingScreenshot_working_copy.height() - 1 ? pendingScreenshot_working_copy.height() - 1 : maxy + compareOffset - 1;

            /*
            minx = minx - compareOffset - 11 < 0 ? 0 :  minx - compareOffset - 11;
            miny = miny - compareOffset - 11 < 0 ? 0 : miny - compareOffset - 11;
            maxx = maxx + compareOffset + 1 > pendingScreenshot_working_copy.width() - 1 ? pendingScreenshot_working_copy.width() - 1 :maxx + compareOffset + 1;
            maxy = maxy + compareOffset + 1 > pendingScreenshot_working_copy.height() - 1 ? pendingScreenshot_working_copy.height() - 1 : maxy + compareOffset + 1;
            */

            /*
            QPoint topLeft(minx,miny);
            QPoint bottomRight(maxx,maxy);
            QPainter pt;
            pt.begin(&previousScreenshot);
            pt.drawImage(minx,miny, pendingScreenshot_working_copy.copy(minx,miny,maxx-minx+1,maxy-miny+1));
            pt.end();
            */

            //auto end = std::chrono::steady_clock::now();
            //auto diff = end - start;
            //std::cout << std::chrono::duration<double,std::milli>(diff).count() << " ms" <<std::endl;
            //qDebug("12. screenshotsWorker.prepareAndSendScreenshotDiff  Diaforetika screenshot. Antigrafi kai proetoimasia apostolis.");
           //qDebug("Ta screenshot einai diaforetika. minx: %i,maxx: %i, miny: %i, maxy: %i",minx,maxx,miny,maxy);
            outimg=pendingScreenshot_working_copy.copy(minx,miny,maxx-minx+1,maxy-miny+1);
            sendScreenshot(outimg,minx,miny);
            //std::cout << "prepareAndSendScreenshotDiff : Egine apostoli screenshot" << std::endl;
       }
       else
       {
           //qDebug("12. screenshotsWorker.prepareAndSendScreenshotDiff  Idia screenshot. Tha steilw empty qimage");
           //to proigoumeno me to trexon einai idia, opote

           //ksanapernw screenshot mexri na yparxei kapoia diafora
           QThread::currentThread()->msleep(50); //ksekourazw tin cpu kai dinw ligo xrono oste na yparksei kapoia diafora
           //QImage qimg(QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId()).toImage());
           //setScreenshot(qimg,protocol->MSG_SCREENSHOT_DIFF_REQUEST);

           sendScreenshot(outimg,0,0);
           //std::cout << "prepareAndSendScreenshotDiff : Apostoli adeiou screenshot" << std::endl;
       }
}

void screenshotsWorker::sendScreenshot(QImage outimg,int x, int y)
{
    try
    {
        //qDebug("13. screenshotsWorker.sendScreenshot  called. Dimiourgia kai compress twn bytes tou image. meta apostoli");

        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        QImage locimg(outimg);
        locimg.save(&buffer,"JPG",this->imageQuality); // writes pixmap into bytes in PNG format

        int nSize = bytes.size();
         //qDebug("## image bytes size uncompressed: %i",nSize);

        QByteArray compressed_bytes = qCompress(bytes,9);
         //qDebug("## image bytes size commpressed: %i",compressed_bytes.size());

         std::array<char,1> cmd;

         std::vector<char> vimgbytes;
         if(pendingMsg.load() == protocol_supervisor->protocol.MSG_SCREENSHOT_DIFF_REQUEST)
         {
             cmd[0] = 's';

             std::vector<char> cx(2);
             intToBytes(x,cx);
             vimgbytes.insert(vimgbytes.end(),cx.begin(),cx.end());
             //qDebug("x to send: %i",x);

             std::vector<char> cy(2);
             intToBytes(y,cy);
             vimgbytes.insert(vimgbytes.end(),cy.begin(),cy.end());
             //qDebug("y to send: %i",y);
         }
         else
         {
             cmd[0] = 'S';
         }

        vimgbytes.insert(vimgbytes.end(), compressed_bytes.begin(),compressed_bytes.end());

        //qDebug("14. screenshotsWorker.sendScreenshot  etoimi eikona pros apostoli. Bytes: %lu", vimgbytes.size());

        if (p2pServer->isP2PCientConnected){
            _sendmsg(p2pServer->activeClientSocket,cmd,vimgbytes);
        }else{
            _sendmsg(protocol_supervisor->protocol.activeSocket,cmd,vimgbytes);
        }
        //qDebug("20. ---> Oloklirwsi apostolis screenshot diff.");

        vimgbytes.clear();
        vimgbytes.swap(vimgbytes);
    }
    catch (std::exception& ex)
    {
        qDebug("EXCEPTION: void screenshotsWorker::sendScreenshot(QImage outimg,int x, int y), What: %s",ex.what());
    }
    catch ( ... )
    {
        qDebug("EXCEPTION: void screenshotsWorker::sendScreenshot(QImage outimg,int x, int y)");
    }
}
