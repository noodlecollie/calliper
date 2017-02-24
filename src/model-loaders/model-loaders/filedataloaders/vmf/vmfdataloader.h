#ifndef VMFDATALOADER_H
#define VMFDATALOADER_H

#include "model-loaders_global.h"
#include "model-loaders/filedataloaders/base/basefileloader.h"
#include <QJsonDocument>
#include <QVector>
#include <QString>

namespace Model
{
    class MapFileDataModel;
    class TexturedWinding;
}

namespace ModelLoaders
{
    class MODELLOADERSSHARED_EXPORT VmfDataLoader : public BaseFileLoader
    {
    public:
        VmfDataLoader(Model::MapFileDataModel* dataModel);

        virtual FileType type() const override;

        Model::MapFileDataModel* vmfDataModel();
        const Model::MapFileDataModel* vmfDataModel() const;

        virtual SuccessCode load(const QString &filePath, QString *errorString) override;
        virtual SuccessCode save(const QString &filePath, QString *errorString) override;

    private:
        void createBrushes(const QJsonDocument &doc);
        void createBrushForSolid(const QJsonObject& solid);
        Model::TexturedWinding* createSide(const QJsonObject& side, int brushId);
        void addError(int brushId, const QString& error);
        void clearInternalState();

        SuccessCode m_iSuccess;
        QVector<QString> m_Errors;
    };
}

#endif // VMFDATALOADER_H
