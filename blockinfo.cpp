#include "blockinfo.h"

BlockInfo::BlockInfo(int maxTextLength, QMap< int, QMap<QString, QString> > inputs, QMap< int, QMap<QString, QString> > outputs, QFileInfo path) :
    maxTextLength(maxTextLength), inputs(inputs), outputs(outputs), path(path)
{

}

BlockInfo::BlockInfo() {}

int BlockInfo::getMaxTextLength()
{
    return this->maxTextLength;
}

QMap< int, QMap<QString, QString> > BlockInfo::getInputs()
{
    return this->inputs;
}

QMap< int, QMap<QString, QString> > BlockInfo::getOutputs()
{
    return this->outputs;
}

QFileInfo BlockInfo::getPath()
{
    return this->path;
}

QString BlockInfo::getBlockCall()
{
    return this->blockCall;
}

void BlockInfo::setMaxTextLength(int maxTextLength)
{
    this->maxTextLength = maxTextLength;
}

void BlockInfo::setInputs(QMap< int, QMap<QString, QString> > inputs)
{
    this->inputs = inputs;
}

void BlockInfo::setOutputs(QMap< int, QMap<QString, QString> > outputs)
{
    this->outputs = outputs;
}

void BlockInfo::setPath(QFileInfo path)
{
    this->path = path;
}

void BlockInfo::parseXML(QFileInfo path)
{
    QFile f(path.filePath());

    QMap< int, QMap<QString, QString> > inputs;
    QMap< int, QMap<QString, QString> > outputs;
    QMap< int, QMap<QString, QString> > locals;
    int maxTextLength = 0;
    QString blockCall;

    if(!f.open(QIODevice::ReadOnly))
    {
        qDebug() <<  QString("%1 - Unable to open.").arg(f.fileName());
    }
    else
    {
        QString content = QString(f.readAll());

        QXmlStreamReader reader(content);
        QString currGroup = "";
        QString lastGroup = "";
        QString currElement = "";
        int currItemID = 0;
        QMap <QString, QString> currItem;

        while(!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();

            if (reader.isStartElement())
            {
                currElement = reader.name().toString();
                //qDebug() << "element name:" << currElement;
                if(reader.name().toString() == QString("Inputs")
                        || reader.name().toString() == QString("Outputs")
                        || reader.name().toString() == QString("Locals"))
                    currGroup = reader.name().toString();
                if(lastGroup == "")
                    lastGroup = currGroup;
            }

            //here we are inside the element, so if it is not empty, we can read the element's value
            if(reader.isCharacters() && !reader.isWhitespace())
            {

                if(currItem.contains(currElement))
                {
                    if(lastGroup == QString("Inputs"))
                        inputs[currItemID] = currItem;
                    else if(lastGroup == QString("Outputs"))
                        outputs[currItemID] = currItem;
                    else if(lastGroup == QString("Locals"))
                        locals[currItemID] = currItem;
                    currItemID++;
                    currItem = QMap <QString, QString>();
                    if(currGroup != lastGroup)
                    {
                        lastGroup = currGroup;
                        currItemID = 0;
                    }

                }
                currItem[currElement] = reader.text().toString();
            }
        }
        if(lastGroup == QString("Inputs"))
            inputs[currItemID] = currItem;
        else if(lastGroup == QString("Outputs"))
            outputs[currItemID] = currItem;
        else if(lastGroup == QString("Locals"))
            locals[currItemID] = currItem;

        QString blockName(path.fileName().left(path.fileName().length() - 4));
        blockCall = blockName + "(";
        for(int i = 0; i < inputs.count(); ++i)
        {
            int len = inputs[i]["Name"].length() + inputs[i]["STType"].length() + 3;
            if(len > maxTextLength)
                maxTextLength = len;
            blockCall += inputs[i]["Name"] + ", ";
        }
        if(inputs.count() != 0)
            blockCall = blockCall.left(blockCall.length() - 2);
        blockCall += ")";

        for(int i = 0; i < outputs.count(); ++i)
        {
            int len = outputs[i]["Name"].length() + outputs[i]["STType"].length() + 3;
            if(len > maxTextLength)
                maxTextLength = len;
        }
    }

    this->blockCall = blockCall;
    this->maxTextLength = maxTextLength;
    this->inputs = inputs;
    this->outputs = outputs;
    this->path = path;
}

void BlockInfo::saveImage()
{
    int maxTextLength = this->getMaxTextLength();
    QMap< int, QMap<QString, QString> > inputs = this->getInputs();
    QMap< int, QMap<QString, QString> > outputs = this->getOutputs();
    QFileInfo path = this->getPath();

    int backgroundWidth = 884;
    int pointSize = 60;
    int width = backgroundWidth + 264 * 2 + 2 * (32 * pointSize / 50) * maxTextLength;
    //int width = 3000;
    int height = 404 + (120 * (qMax(inputs.count(), outputs.count())));
    QString blockName = path.fileName().left(path.fileName().length() - 4);

    QImage image(width, height, QImage::Format_RGBA8888);
    QPainter painter(&image);
    painter.fillRect(image.rect(), Qt::white);

    QImage drawer(":/images/top.png");
    QRectF source(0.0, 0.0, backgroundWidth, 202.0);
    QRectF target((width - backgroundWidth) / 2, 0.0, backgroundWidth, 202.0);
    painter.drawImage(target, drawer, source);

    drawer = QImage(":/images/bottom.png");
    source = QRectF(0.0, 0.0, backgroundWidth, 202.0);
    target = QRectF((width - backgroundWidth) / 2, 202 + (120 * (qMax(inputs.count(), outputs.count()))), backgroundWidth, 202.0);
    painter.drawImage(target, drawer, source);

    drawer = QImage(":/images/middle_gray.png");
    source = QRectF(0.0, 0.0, 884.0, 10.0);
    target = QRectF((width - backgroundWidth) / 2, 202, backgroundWidth, (120 * (qMax(inputs.count(), outputs.count()))));
    painter.drawImage(target, drawer, source);

    float middleHeight = qMin(353, (120 * (qMax(inputs.count(), outputs.count()))));
    drawer = QImage(":/images/middle.png");
    source = QRectF(0.0, 0.0, 628.0, 353.0);
    target = QRectF((width - (628 * (middleHeight/353.0))) / 2, (height - middleHeight) / 2, 628 * (middleHeight/353.0), middleHeight);
    painter.drawImage(target, drawer, source);

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(pointSize);
    //QFont font("Arial", 50, -1, false);
    QPen pen(QColor(0, 0, 0, 255));
    painter.setFont(font);
    painter.setPen(pen);

    //for(auto elem = inputs.begin(); elem != inputs.end(); ++elem)
    int textSize = (32 * pointSize / 50) * maxTextLength;
    int inOutSpacing = ((width - backgroundWidth) / 2) - 258 - textSize;
    for(int i = 0; i < inputs.count(); ++i)
    {
        QString inputInfo = QString("%1 (%2)").arg(inputs[i]["Name"], inputs[i]["STType"]);
        painter.drawText(QRectF(inOutSpacing, 240 + (120 * i), textSize, 200), Qt::AlignRight, inputInfo);

        drawer = QImage(":/images/arrow_point_right.png");
        source = QRectF(0.0, 0.0, 245.0, 62.0);
        target = QRectF(inOutSpacing + textSize + 13, 246 + (120 * i), 245, 62);
        painter.drawImage(target, drawer, source);
    }
    for(int i = 0; i < outputs.count(); ++i)
    {
        QString outputInfo = QString("%1 (%2)").arg(outputs[i]["Name"], outputs[i]["STType"]);
        painter.drawText(QRectF(width - inOutSpacing - textSize, 240 + (120 * i), textSize, 200), Qt::AlignLeft, outputInfo);

        drawer = QImage(":/images/arrow_point_right.png");
        source = QRectF(0.0, 0.0, 245.0, 62.0);
        target = QRectF(width - inOutSpacing - textSize - 12 - 246, 246 + (120 * i), 245, 62);
        painter.drawImage(target, drawer, source);
    }

    font.setPointSize(60);
    pen.setColor(QColor(150, 0, 0, 255));
    painter.setFont(font);
    painter.setPen(pen);

    int blockNameWidth = 32*60/50 * (blockName.length());
    int blockNameHeight = 65;
    QImage titleImage(blockNameWidth, 90, QImage::Format_RGBA8888);
    QPainter painterTitle(&titleImage);
    painterTitle.setFont(font);
    painterTitle.setPen(pen);
    painterTitle.fillRect(titleImage.rect(), QColor(226, 226, 226, 255));
    painterTitle.drawText(QRect(0, 0, blockNameWidth, blockNameHeight), Qt::AlignHCenter|Qt::AlignBottom, blockName);
    painterTitle.end();

    source = QRectF(0.0, 0.0, blockNameWidth, blockNameHeight);

    blockNameWidth = qMin(21 * 32 * 60 / 50, blockNameWidth);
    target = QRectF((width - blockNameWidth) / 2, 130 - (blockNameHeight / 2), blockNameWidth, blockNameHeight);

    painter.drawImage(target, titleImage, source);

    QDir imageDir("./blockimages");
    if(!imageDir.exists())
        imageDir.mkpath(".");

    qDebug() << QString("%1.png created!").arg(blockName);
    image.save(QString("%1/%2.png").arg(imageDir.path(), blockName));
}
