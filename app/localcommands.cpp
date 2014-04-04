#include "localcommands.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QString>

using namespace ICONSOLE_NAMESPACE;

int ccb_DumpGlobalStyleSheet(const CommandSenderInfo &info, const QStringList &args, QVariant &output)
{
    Q_UNUSED(args)
    Q_UNUSED(output)
    info.writeWarning("Global style sheet:\n");
    info.writeMessage(qApp->styleSheet());
    
    return NGlobalCmd::CCR_OK;
}

int ccb_RefreshStyleSheet(const CommandSenderInfo &info, const QStringList &args, QVariant &output)
{
    Q_UNUSED(args)
    Q_UNUSED(output)
    
    info.writeMessage("Refreshing style sheet...\n");
    
    if ( applyStyleSheet() )
    {
        info.writeMessage("Done.\n");
        return NGlobalCmd::CCR_OK;
    }
    
    info.writeWarning("Setting style sheet failed.\n");
    return -1;
}

bool applyStyleSheet()
{
    // Apply style sheet.
    QDir dir(qApp->applicationDirPath());
    if ( dir.cd("resource") && dir.exists("manifest.qss") )
    {
        QFile stylesheet(dir.filePath("manifest.qss"));
        stylesheet.open(QFile::ReadOnly);
        QString setSheet = QLatin1String(stylesheet.readAll());
        qApp->setStyleSheet(setSheet);
        return true;
    }
    else return false;
}
