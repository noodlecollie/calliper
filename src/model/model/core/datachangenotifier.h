#ifndef DATACHANGENOTIFIER_H
#define DATACHANGENOTIFIER_H

#include "model_global.h"
#include <QObject>

namespace Model
{
    class MODELSHARED_EXPORT DataChangeNotifier : public QObject
    {
        Q_OBJECT
    public:
        explicit DataChangeNotifier(QObject *parent = 0);

        bool dataHasChanged() const;

    signals:
        void dataChanged(int propertyIndex);
        void dataReset();

    public slots:
        void notifyDataReset();
        void notifyDataChanged(int propertyIndex = -1);

    protected:
        // Connects CHANGED source->this, RESET this->source.
        void connectDataChangedSignals(DataChangeNotifier* source);

    private:
        bool m_bDataChanged;
    };
}

#endif // DATACHANGENOTIFIER_H
