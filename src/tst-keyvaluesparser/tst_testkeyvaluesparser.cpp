#include <QString>
#include <QtTest>
#include "file-formats/keyvalues/keyvaluesparser.h"
#include <QFile>

class TestKeyValuesParser : public QObject
{
    Q_OBJECT

public:
    TestKeyValuesParser();

private Q_SLOTS:
    void testSampleVmt1();
    void testSampleVmt2();

private:
    bool loadResource(const QString &filename, QByteArray& data)
    {
        QFile file(filename);
        if ( !file.open(QIODevice::ReadOnly) )
        {
            return false;
        }

        data = file.readAll();
        file.close();
        return true;
    }
};

TestKeyValuesParser::TestKeyValuesParser()
{
}

void TestKeyValuesParser::testSampleVmt1()
{
    QByteArray data;
    QVERIFY2(loadResource(":/resource/materials.models.props_coalmine.replacements.vmt", data),
             "Could not load test resource.");

    FileFormats::KeyValuesParser parser(data);
    QString error;
    QJsonDocument doc = parser.toJsonDocument(&error);
    if ( !error.isNull() )
    {
        qDebug() << "Import error:" << error;
    }

    QVERIFY2(!doc.isNull(), "JSON document should not be null.");
}

void TestKeyValuesParser::testSampleVmt2()
{
    QByteArray data;
    QVERIFY2(loadResource(":/resource/materials.models.items.bullion.vmt", data),
             "Could not load test resource.");

    FileFormats::KeyValuesParser parser(data);
    QString error;
    QJsonDocument doc = parser.toJsonDocument(&error);
    if ( !error.isNull() )
    {
        qDebug() << "Import error:" << error;
    }

    QVERIFY2(doc.isNull(), "Import should fail due to incomplete key.");
}

QTEST_APPLESS_MAIN(TestKeyValuesParser)

#include "tst_testkeyvaluesparser.moc"
