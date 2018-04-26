#ifndef BLOCKINFO_H
#define BLOCKINFO_H

#include <QMap>
#include <QFileInfo>
#include <QPainter>
#include <QFontDatabase>
#include <QXmlStreamReader>
#include <QDir>
#include <QDebug>

class BlockInfo
{
public:
    BlockInfo(int maxTextLength, QMap< int, QMap<QString, QString> > inputs, QMap< int, QMap<QString, QString> > outputs, QFileInfo path);
    BlockInfo();

    int getMaxTextLength();
    QMap< int, QMap<QString, QString> > getInputs();
    QMap< int, QMap<QString, QString> > getOutputs();
    QFileInfo getPath();

    void setMaxTextLength(int maxTextLength);
    void setInputs(QMap< int, QMap<QString, QString> > inputs);
    void setOutputs(QMap< int, QMap<QString, QString> > outputs);
    void setPath(QFileInfo path);

    void parseXML(QFileInfo path);
    void saveImage();

private:
    int maxTextLength;
    QMap< int, QMap<QString, QString> > inputs;
    QMap< int, QMap<QString, QString> > outputs;
    QFileInfo path;
};

#endif // BLOCKINFO_H
