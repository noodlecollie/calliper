#include "hierarchicalobject.h"
#include "callipermath.h"
#include <cmath>
#include "jsonutil.h"

HierarchicalObject::HierarchicalObject(HierarchicalObject *parent) : QObject(parent)
{
    initDefaults();
}

HierarchicalObject::~HierarchicalObject()
{

}

void HierarchicalObject::initDefaults()
{
    m_vecPosition = QVector3D(0,0,0);
    m_angAngles = EulerAngle(0,0,0);
    m_bMatricesStale = true;
    m_vecScale = QVector3D(1,1,1);
}

QVector3D HierarchicalObject::position() const
{
    return m_vecPosition;
}

void HierarchicalObject::setPosition(const QVector3D &pos)
{
    if ( m_vecPosition == pos ) return;

    m_vecPosition = pos;
    m_bMatricesStale = true;
    emit orientationChanged();
}

void HierarchicalObject::rebuildMatrices() const
{
    rebuildLocalToParent();
    m_matParentToLocal = m_matLocalToParent.inverted();
    m_bMatricesStale = false;
}

void HierarchicalObject::rebuildLocalToParent() const
{
    // To get from local (model) space to world space,
    // we perform transforms forward.
    // To get from world space to camera space we must
    // perform the camera transforms backward - see Camera class.
    m_matLocalToParent = Math::matrixTranslate(m_vecPosition)
            * Math::matrixOrientation(m_angAngles)
            * Math::matrixScale(m_vecScale);
}

QMatrix4x4 HierarchicalObject::parentToLocal() const
{
    if ( m_bMatricesStale )
        rebuildMatrices();

    return m_matParentToLocal;
}

QMatrix4x4 HierarchicalObject::localToParent() const
{
    if ( m_bMatricesStale )
        rebuildMatrices();

    return m_matLocalToParent;
}

EulerAngle HierarchicalObject::angles() const
{
    return m_angAngles;
}

void HierarchicalObject::setAngles(const EulerAngle &angle)
{
    if ( m_angAngles == angle ) return;

    m_angAngles = angle;
    normaliseAngles();
    clampAngles();
    m_bMatricesStale = true;
    emit orientationChanged();
}

void HierarchicalObject::clampAngles()
{
    if ( m_angAngles.pitch() < -90.0f )
        m_angAngles.setPitch(-90.0f);

    else if ( m_angAngles.pitch() > 90.0f )
        m_angAngles.setPitch(90.0f);

    if ( m_angAngles.roll() < -180.0f )
        m_angAngles.setRoll(-180.0f);

    else if ( m_angAngles.roll() > 180.0f )
        m_angAngles.setRoll(180.0f);

    m_angAngles.setYaw(std::fmod(m_angAngles.yaw(), 360.0f));
}

void HierarchicalObject::normaliseAngles()
{
    // Normalise pitch.
    if ( m_angAngles.pitch() > 180.0f )
        m_angAngles.setPitch(-360.0f + m_angAngles.pitch());

    // Normalise roll.
    if ( m_angAngles.roll() > 180.0f )
        m_angAngles.setRoll(-360.0f + m_angAngles.roll());
}

void HierarchicalObject::translate(const QVector3D &trans)
{
    setPosition(position() + (Math::matrixOrientation(m_angAngles)*trans));
}

QMatrix4x4 HierarchicalObject::rootToLocal() const
{
    QMatrix4x4 mat = parentToLocal();
    HierarchicalObject* obj = qobject_cast<HierarchicalObject*>(parent());
    while (obj)
    {
        mat = mat * obj->parentToLocal();
        obj = qobject_cast<HierarchicalObject*>(obj->parent());
    }

    return mat;
}

void HierarchicalObject::lookAtLocal(const QVector3D &localSpacePos)
{
    setAngles(Math::vectorToAngleSimple(localSpacePos));
}

void HierarchicalObject::lookAtParent(const QVector3D &parentSpacePos)
{
    setAngles(Math::vectorToAngleSimple(parentSpacePos - m_vecPosition));
}

void HierarchicalObject::lookAtGlobal(const QVector3D &globalSpacePos)
{
    setAngles(Math::vectorToAngleSimple(rootToParent(globalSpacePos) - m_vecPosition));
}

QVector3D HierarchicalObject::scale() const
{
    return m_vecScale;
}

void HierarchicalObject::setScale(const QVector3D &vec)
{
    if ( !scalable() || m_vecScale == vec || m_vecScale.isNull() ) return;

    m_vecScale = vec;
    m_bMatricesStale = true;
    emit orientationChanged();
}

bool HierarchicalObject::scalable() const
{
    return true;
}

QVector3D HierarchicalObject::rootToLocal(const QVector3D &vec, bool direction) const
{
    return (rootToLocal() * QVector4D(vec, direction ? 0 : 1)).toVector3D();
}

QVector3D HierarchicalObject::localToRoot(const QVector3D &vec, bool direction) const
{
    return (rootToLocal().inverted() * QVector4D(vec, direction ? 0 : 1)).toVector3D();
}

QVector3D HierarchicalObject::parentToLocal(const QVector3D &vec, bool direction) const
{
    return (parentToLocal() * QVector4D(vec, direction ? 0 : 1)).toVector3D();
}

QVector3D HierarchicalObject::localToParent(const QVector3D &vec, bool direction) const
{
    return (localToParent() * QVector4D(vec, direction ? 0 : 1)).toVector3D();
}

QVector3D HierarchicalObject::parentToRoot(const QVector3D &vec, bool direction) const
{
    return (rootToLocal().inverted() * parentToLocal() * QVector4D(vec, direction ? 0 : 1)).toVector3D();
}

QVector3D HierarchicalObject::rootToParent(const QVector3D &vec, bool direction) const
{
    return (localToParent() * rootToLocal() * QVector4D(vec, direction ? 0 : 1)).toVector3D();
}

bool HierarchicalObject::serialiseToJson(QJsonObject &obj) const
{
    // Set our identifier.
    obj.insert(ISerialisable::KEY_IDENTIFIER(), QJsonValue(HierarchicalObject::serialiseIdentifier()));

    // Store the relevant properties.
    QJsonArray arrPos;
    JsonUtil::vector3ToJsonArray<QVector3D>(position(), arrPos);
    obj.insert("position", QJsonValue(arrPos));

    QJsonArray arrAng;
    JsonUtil::vector3ToJsonArray<EulerAngle>(angles(), arrAng);
    obj.insert("angles", QJsonValue(arrAng));

    QJsonArray arrScl;
    JsonUtil::vector3ToJsonArray<QVector3D>(scale(), arrScl);
    obj.insert("scale", QJsonValue(arrScl));

    return true;
}

QString HierarchicalObject::serialiseIdentifier() const
{
    return staticMetaObject.className();
}

HierarchicalObject::HierarchicalObject(const QJsonObject &serialisedData, HierarchicalObject *parent) : QObject(parent)
{
    initDefaults();

    // Make sure this object identifies us.
    if ( !validateIdentifier(serialisedData, HierarchicalObject::serialiseIdentifier()) )
    {
        return;
    }

    QJsonArray arrPos = serialisedData.value("position").toArray();
    if ( arrPos.count() >= 3 )
    {
        setPosition(JsonUtil::jsonArrayToVector3<QVector3D>(arrPos));
    }
    else
    {
        setPosition(QVector3D());
    }

    QJsonArray arrAng = serialisedData.value("angles").toArray();
    if ( arrAng.count() >= 3 )
    {
        setAngles(JsonUtil::jsonArrayToVector3<EulerAngle>(arrAng));
    }
    else
    {
        setAngles(EulerAngle());
    }

    QJsonArray arrScl = serialisedData.value("scale").toArray();
    if ( arrScl.count() >= 3 )
    {
        setScale(JsonUtil::jsonArrayToVector3<QVector3D>(arrScl));
    }
    else
    {
        setScale(QVector3D(1,1,1));
    }
}

QVector3D HierarchicalObject::globalPosition() const
{
    return (rootToLocal().inverted() * QVector4D(0,0,0,1)).toVector3D();
}
