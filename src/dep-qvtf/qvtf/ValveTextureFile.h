/*    LoadValveTextureFile - load Valve Texture Format files in Qt4 applications
 *    Copyright (C) 2014  Mathias Panzenböck
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef QVTF_LOAD_VALVE_TEXTURE_FILE_H
#define QVTF_LOAD_VALVE_TEXTURE_FILE_H

#include <QImageIOPlugin>
#include "VTFLib/src/VTFFile.h"

#define ValveTextureFilePlugin_iid "org.calliper.ValveTextureFilePlugin"

class ValveTextureFilePlugin : public QImageIOPlugin {
    Q_OBJECT
    Q_CLASSINFO("author", "Mathias Panzenböck")
    Q_CLASSINFO("url", "https://github.com/panzi/qvtf")
    Q_PLUGIN_METADATA(IID ValveTextureFilePlugin_iid FILE "ValveTextureFilePlugin.json")

public:

    Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
    QImageIOHandler* create(QIODevice *device, const QByteArray &format) const;
    QStringList keys() const;
};

class ValveTextureFileHandler : public QImageIOHandler {

public:
    ValveTextureFileHandler();
    ~ValveTextureFileHandler();

    bool canRead() const;
    int currentImageNumber() const;
    QRect currentImageRect() const;
    int imageCount() const;
    bool jumpToImage(int imageNumber);
    bool jumpToNextImage();
    int loopCount() const;
    int nextImageDelay() const;
    QVariant option(ImageOption option) const;
    bool read(QImage *image);
    bool supportsOption(ImageOption option) const;

    static bool canRead(QIODevice *device);

    bool read();

private:

    enum State {
        Ready,
        Read,
        Error
    };

    State state;
    int currentFrame;
    VTFLib::CVTFFile vtf;
};

#endif
