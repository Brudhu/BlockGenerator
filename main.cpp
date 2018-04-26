#include <QGuiApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QDirIterator>
#include <QDebug>
#include <QMap>
#include <QXmlStreamReader>

#include "blockinfo.h"


int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    QGuiApplication::setApplicationName("Block Generator");
    QGuiApplication::setApplicationVersion("1.0");

    qSetMessagePattern("[%{type}] %{message}");

    QCommandLineParser parser;
    parser.setApplicationDescription("Qt application to read XML files of variables of blocks for the Proteo PC "
                                     "and generate a PNG image displaying inputs and outputs of the block. "
                                     "It can be used with a single file or a directory - in this case, it will "
                                     "generate blocks for every file inside the directory.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("path", QCoreApplication::translate("main", "Path to file or directory to replace strings."));

    QCommandLineOption r(QStringList() << "r" << "R" << "recursive", QCoreApplication::translate("main", "Generate PNG image in directories and files recursively"));
    parser.addOption(r);
    parser.process(a);

    const QStringList args = parser.positionalArguments();
    if(args.length() != 1)
    {
        parser.showHelp();
    }
    bool recursive = parser.isSet(r);
    QFileInfo path(args.at(0));

    if(path.isDir()) // If it's a directory, enter and do the regex replacing in all the files.
    {
        qDebug() << "Argument \"Path\" is a directory.";

        // Add the recursive option
        QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
        if(recursive)
            flags = QDirIterator::Subdirectories;

        QDirIterator dirIt(path.path(), flags);

        while(dirIt.hasNext())
        {
            QString filePathStr = dirIt.next();
            QFileInfo filePath(filePathStr);
            if(!filePath.isFile() || filePath.suffix() != "var")
            {
                continue;
            }

            BlockInfo block;
            block.parseXML(filePath);
            block.saveImage();
        }
    }
    else if(path.isFile())
    {
        qDebug() << "Argument \"Path\" is a file.";

        BlockInfo block;
        block.parseXML(path);
        block.saveImage();
    }

    return 0;
}

