#include <QString>
#include <QtTest>
#include "keyvalues/keyvaluesparser.h"

namespace
{
    const char* comment = "/* Comment*/";

    const char* vmt1 =
    "\"VertexLitGeneric\"\n"
    "{\n"
    "    \"$basetexture\" \"Models/props_spytech/raygun001\"\n"
    "    \"$selfillum\" 	1\n"
    "\n"
    "    // Use separate self-illum mask on Pre DX9 hardware\n"
    "    \">=DX90\"\n"
    "    {\n"
    "        \"$baseTexture\" 		\"Models/props_spytech/raygun001_noalpha\"\n"
    "        \"$selfillummask\" 	\"Models/props_spytech/raygun001_selfillum\"\n"
    "    }\n"
    "\n"
    "    // Use redundant self-illum in base alpha on Pre DX9 hardware\n"
    "    \"<DX90\"\n"
    "    {\n"
    "        \"$baseTexture\" 	\"Models/props_spytech/raygun001\"\n"
    "    }\n"
    "}\n"
    ;
}

class TestKeyValuesParser : public QObject
{
    Q_OBJECT

public:
    TestKeyValuesParser();

private Q_SLOTS:
    void testSampleVmt1();
};

TestKeyValuesParser::TestKeyValuesParser()
{
}

void TestKeyValuesParser::testSampleVmt1()
{
    QByteArray data(vmt1);
    FileFormats::KeyValuesParser parser(data);
    QString error;
    QJsonDocument doc = parser.toJsonDocument(&error);

    QVERIFY2(!doc.isNull(), "JSON document should not be null.");
}

QTEST_APPLESS_MAIN(TestKeyValuesParser)

#include "tst_testkeyvaluesparser.moc"
