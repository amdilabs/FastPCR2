#include "configuration.h"

const Configuration::Alignment Configuration::defaultAlignment = RiseStart;
const Configuration::RampData Configuration::defaultRampData = Peak1090;
const Configuration::RampFitting Configuration::defaultRampFitting = TwoPoint;
const double Configuration::defaultStep = 15.0;
const double Configuration::defaultLength = 5.0;

Configuration::Configuration (void) :
  m_alignment (defaultAlignment),
  m_rampData (defaultRampData),
  m_rampFitting (defaultRampFitting),
  m_step (defaultStep),
  m_length (defaultLength)
{
}

Configuration::Configuration (const QJsonObject &object) :
  m_alignment (alignmentFromJson (object.value ("alignment"))),
  m_rampData (rampDataFromJson (object.value ("rampData"))),
  m_rampFitting (rampFittingFromJson (object.value ("rampFitting"))),
  m_step (object.value ("step").toDouble (defaultStep)),
  m_length (object.value ("length").toDouble (defaultLength))
{
}

Configuration::Alignment Configuration::alignment (void) const
{ return m_alignment;
}

Configuration::RampData Configuration::rampData (void) const
{ return m_rampData;
}

Configuration::RampFitting Configuration::rampFitting (void) const
{ return m_rampFitting;
}

double Configuration::step (void) const
{ return m_step;
}

double Configuration::length (void) const
{ return m_length;
}

void Configuration::setAlignment (const Alignment alignment)
{ m_alignment = alignment;
}

void Configuration::setRampData (const RampData rampData)
{ m_rampData = rampData;
}

void Configuration::setRampFitting (const RampFitting rampFitting)
{ m_rampFitting = rampFitting;
}

void Configuration::setStep (const double step)
{ m_step = step;
}

void Configuration::setLength (const double length)
{ m_length = length;
}

QJsonObject Configuration::toJson (void) const
{ QJsonObject object;
  object.insert ("alignment", alignmentToJson (m_alignment));
  object.insert ("rampData", rampDataToJson (m_rampData));
  object.insert ("rampFitting", rampFittingToJson (m_rampFitting));
  object.insert ("step", m_step);
  object.insert ("length", m_length);
  return object;
}

Configuration::Alignment Configuration::alignmentFromJson (const QJsonValue &value)
{ QString str = value.toString ();
  if (str == "none") return None;
  if (str == "riseStart") return RiseStart;
  if (str == "peak") return Peak;
  return defaultAlignment;
}

Configuration::RampData Configuration::rampDataFromJson (const QJsonValue &value)
{ QString str = value.toString ();
  if (str == "peak1090") return Peak1090;
  if (str == "fromStart") return FromStart;
  if (str == "fromPeak") return FromPeak;
  return defaultRampData;
}

Configuration::RampFitting Configuration::rampFittingFromJson (const QJsonValue &value)
{ QString str = value.toString ();
  if (str == "twoPoint") return TwoPoint;
  if (str == "linearRegression") return LinearRegression;
  return defaultRampFitting;
}

QJsonValue Configuration::alignmentToJson (const Alignment alignment)
{ switch (alignment)
  { case None: return QJsonValue ("none");
    case RiseStart: return QJsonValue ("riseStart");
    case Peak: return QJsonValue ("peak");
  }

  return alignmentToJson (defaultAlignment);
}

QJsonValue Configuration::rampDataToJson (const RampData rampData)
{ switch (rampData)
  { case Peak1090: return QJsonValue ("peak1090");
    case FromStart: return QJsonValue ("fromStart");
    case FromPeak: return QJsonValue ("fromPeak");
  }

  return rampDataToJson (defaultRampData);
}

QJsonValue Configuration::rampFittingToJson (const RampFitting rampFitting)
{ switch (rampFitting)
  { case TwoPoint: return QJsonValue ("twoPoint");
    case LinearRegression: return QJsonValue ("linearRegression");
  }

  return rampFittingToJson (defaultRampFitting);
}
