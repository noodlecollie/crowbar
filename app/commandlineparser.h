#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#define CLA_DEBUGGING   "-debug"
#define CLA_LOGGING     "-log"

#include <QObject>

class CommandLineParser : public QObject
{
    Q_OBJECT
public:
    explicit CommandLineParser(QObject *parent = 0);

    bool Debugging();
    bool Logging();
    
signals:
    
public slots:
    void ParseArguments(int argc, char **argv);

private:
    bool m_bDebugging;
    bool m_bLogging;
};

#endif // COMMANDLINEPARSER_H
