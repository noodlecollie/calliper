#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <QStack>
#include <QMatrix4x4>

class MatrixStack
{
public:
    MatrixStack();
    void push();
    void pop();
    int count() const;
    void clear();

    QMatrix4x4& top();
    const QMatrix4x4& top() const;

private:
    QStack<QMatrix4x4>  m_Stack;
};

#endif // MATRIXSTACK_H
