#include "matrixstack.h"

MatrixStack::MatrixStack()
{
    // Start with an identity matrix.
    m_Stack.push(QMatrix4x4());
}

void MatrixStack::push()
{
    // Make a copy of our current top matrix.
    m_Stack.push(m_Stack.top());
}

void MatrixStack::pop()
{
    // If we've only got one matrix, return.
    if ( m_Stack.count() < 2 ) return;

    // Remove one matrix from the stack.
    m_Stack.pop();
}

int MatrixStack::count() const
{
    return m_Stack.count();
}

QMatrix4x4& MatrixStack::top()
{
    return m_Stack.top();
}

const QMatrix4x4& MatrixStack::top() const
{
    return m_Stack.top();
}

void MatrixStack::clear()
{
    m_Stack.clear();
    m_Stack.push(QMatrix4x4());
}
