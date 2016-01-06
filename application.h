#ifndef APPLICATION_H
#define APPLICATION_H

#include <QList>
#include <QPointer>

class MapDocument;
class MainWindow;
class BaseTool;

class Application
{
public:
    Application(MainWindow* win);
    ~Application();

    static void initialise(MainWindow* win);
    static void shutdown();

    MapDocument* newDocument();
    MapDocument* document(int index) const;
    int documentCount() const;
    void closeDocument(int index);
    void clearDocuments();
    QList<MapDocument*> documents() const;

    MainWindow* mainWindow() const;

    BaseTool* tool(int index) const;
    BaseTool* tool(const QString &name) const;
    int toolCount() const;

private:
    void createTools();

    QList<MapDocument*>     m_Documents;
    int m_iDocumentsCreated;

    QPointer<MainWindow>    m_pMainWindow;

    QList<BaseTool*>    m_Tools;
};

Application* application();

#endif // APPLICATION_H
