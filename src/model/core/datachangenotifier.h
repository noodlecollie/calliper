#ifndef DATACHANGENOTIFIER_H
#define DATACHANGENOTIFIER_H

#include "model_global.h"
#include <QObject>

namespace Model
{
    class DataChangeNotifier : public QObject
    {
        Q_OBJECT
    public:
        explicit DataChangeNotifier(QObject *parent = 0);

        bool dataHasChanged() const;

    signals:
        void dataChanged(int propertyIndex = -1);
        void dataReset();

    public slots:
        void notifyDataChanged();
        void notifyDataReset();

    protected:
        // Connects CHANGED source->this, RESET this->source.
        void connectDataChangedSignals(DataChangeNotifier* source);
        void notifyDataChanged(int propertyIndex);

    private:
        bool m_bDataChanged;
    };
}

#endif // DATACHANGENOTIFIER_H
