#include "digibytelengthvalidator.h"

DigiByteLengthValidator::DigiByteLengthValidator(QObject *parent)
    : QValidator{parent}
{
    m_maxBytes = 32;
    m_limitLength = true;
    QRegularExpression other("\\s+");
    re.swap(other);
}

QValidator::State DigiByteLengthValidator::validate(QString &input, int &pos) const
{
    // bool res = re.match(input).hasMatch();
    input = input.trimmed();
    QByteArray utf8Data = input.toUtf8();
    qInfo() << "*********************";
    qInfo() << "data:" << input << "byte:" << utf8Data.size() << "max:" << m_maxBytes;
    qInfo() << "*********************";
    if((!m_limitLength || utf8Data.size() <= m_maxBytes) && !input.isEmpty())
        return Acceptable;
    else if(input.isEmpty())
        return Intermediate;
    else
        return Invalid;
}

void DigiByteLengthValidator::setMaxBytes(unsigned int max)
{
    m_maxBytes = max;
}

void DigiByteLengthValidator::setLimitChar(bool blimit)
{
    m_limitLength = blimit;
}
