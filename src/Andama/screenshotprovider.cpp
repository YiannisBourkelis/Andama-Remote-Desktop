#ifndef ANDAMA_SERVICE_MODE //AndamaService service/daemon does not use qml related code
#include <QPainter>
#include "screenshotprovider.h"
#include "helperfuncs.h"
#include "../Shared/AndamaHeaders/byte_functions.h"

ScreenshotProvider::ScreenshotProvider(): QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage ScreenshotProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (currentFrame.isNull()){
        qDebug("nul pixmap");
        return QImage(":/images/images/advertise_here.png");
    }
    int width = currentFrame.width();
    int height = currentFrame.height();

    if (size)
        *size = QSize(width, height);
   return QImage(currentFrame.toImage());

}

bool ScreenshotProvider::setFrame(const std::vector<char>& compressedBuf)
{
    Q_ASSERT(!compressedBuf.empty());
    return currentFrame.loadFromData( qUncompress( QByteArray( compressedBuf.data(), compressedBuf.size()) ));

}

bool ScreenshotProvider::updateFrame(const std::vector<char>& compressedBuf)
{
    Q_ASSERT(compressedBuf.size() > 4);

    QByteArray image_bytes_uncompressed = qUncompress( QByteArray( compressedBuf.data() + 4, compressedBuf.size() - 4));

    if ( !image_bytes_uncompressed.isEmpty() )
    {
        QPixmap updatePixmap;
        updatePixmap.loadFromData(image_bytes_uncompressed);
        updateFrame( updatePixmap,
                QPoint(
                    bytesToInt(compressedBuf,0,2), //x offset
                    bytesToInt(compressedBuf,2,2)  //y offset
                ));
        return true;
    }
    return false;

}

void ScreenshotProvider::updateFrame(const QPixmap& updateArea, QPoint regionOrg)
{
    Q_ASSERT(! updateArea.isNull());
    QPainter paint(&currentFrame);
    paint.drawPixmap( QRect(regionOrg, updateArea.size()) , updateArea , updateArea.rect());
}
#endif //ANDAMA_SERVICE_MODE
