#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#define CLA_DEBUGGING   "-debug"

#include <QObject>

class CommandLineParser : public QObject
{
    Q_OBJECT
public:
    explicit CommandLineParser(QObject *parent = 0);

    bool Debugging();
    
signals:
    
public slots:
    void ParseArguments(int argc, char **argv);

private:
    bool m_bDebugging;

};

#endif // COMMANDLINEPARSER_H
