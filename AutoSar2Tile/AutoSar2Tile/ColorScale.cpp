#include <algorithm>

//Qt
#include "QString"
#include "QColor"
#include "QFile"

//common
#include "Logger.h"

#include "ColorScale.h"

//默认的颜色表
static std::map<QString, ColorScale*> DefaultColorScales;
unsigned ColorScale::s_number = 0;


/***************************************ColorScaleElement**************************/
ColorScaleElement::ColorScaleElement()
	: m_position(0.0)
	, m_color(Qt::black)
{
	
}

ColorScaleElement::ColorScaleElement(double position, QColor color)
{
	m_position = position;
	m_color = color;
}

double ColorScaleElement::Position() const
{ 
	return m_position; 
}

double& ColorScaleElement::Position()
{ 
	return m_position; 
}

QColor ColorScaleElement::Color() const
{
	return m_color;
}

QColor& ColorScaleElement::Color()
{
	return m_color;
}

bool ColorScaleElement::IsSmaller(const ColorScaleElement& e1, const ColorScaleElement& e2)
{
	return e1.Position() < e2.Position();
}

/**************************************ColorScale**********************************************/

ColorScale::ColorScale()
{
	
	m_name = "";

	m_uniqueID = s_number++;
	m_updated = false;
	m_relative = true;
	m_locked = false;
	m_absoluteMinValue = 0.0;
	m_absoluteRange = 1.0;
}

ColorScale::ColorScale(QString name)
{
	m_name = name;

	m_uniqueID = s_number++;
	m_updated = false;
	m_relative = true;
	m_locked = false;
	m_absoluteMinValue = 0.0;
	m_absoluteRange = 1.0;
}

ColorScale::ColorScale(QString name, QList<ColorScaleElement> elements)
{
	m_name = name;

	m_uniqueID = s_number++;
	m_updated = false;
	m_relative = true;
	m_locked = false;
	m_absoluteMinValue = 0.0;
	m_absoluteRange = 1.0;
	m_elements = elements;

	Update();
}

ColorScale::~ColorScale()
{
}

std::map<QString, ColorScale*> ColorScale::DefualtColorScales()
{	
	if (!DefaultColorScales.empty())
	{
		return DefaultColorScales;
	}

	//默认的几个
	std::map<QString, ColorScale* > result;
	//白->黑
	ColorScale* whiteToBlack = new ColorScale("White->Black");
	whiteToBlack->Insert(ColorScaleElement(0.0, Qt::white));
	whiteToBlack->Insert(ColorScaleElement(1.0, Qt::black));
	result["White->Black"] = whiteToBlack;

	//黑->白
	ColorScale* blackToWhite = new ColorScale("Black->White");
	blackToWhite->Insert(ColorScaleElement(0.0, Qt::black));
	blackToWhite->Insert(ColorScaleElement(1.0, Qt::white));
	result["Black->White"] = blackToWhite;

	//白->黑->红
	ColorScale* whiteBlackRed = new ColorScale("White->Black->Red");
	whiteBlackRed->Insert(ColorScaleElement(0.0, Qt::white));
	whiteBlackRed->Insert(ColorScaleElement(1.0, Qt::red));
	whiteBlackRed->Insert(ColorScaleElement(0.5, Qt::black));
	result["White->Black->Red"] = whiteBlackRed;

	//红->黑->白
	ColorScale* redBlackWhite = new ColorScale("Red->Black->White");
	redBlackWhite->Insert(ColorScaleElement(0.0, Qt::red));
	redBlackWhite->Insert(ColorScaleElement(1.0, Qt::white));
	redBlackWhite->Insert(ColorScaleElement(0.5, Qt::black));
	result["Red->Black->White"] = redBlackWhite;

	//彩虹：红->紫
	ColorScale* rainbowRedToPurple = new ColorScale("Rainbow:Red->Purple");
	rainbowRedToPurple->Insert(ColorScaleElement(0.0, Qt::red));
	rainbowRedToPurple->Insert(ColorScaleElement(1.0, QColor(160, 32, 240)));
	rainbowRedToPurple->Insert(ColorScaleElement(0.166, QColor(255, 165, 0)));
	rainbowRedToPurple->Insert(ColorScaleElement(0.333, Qt::yellow));
	rainbowRedToPurple->Insert(ColorScaleElement(0.5, Qt::green));
	rainbowRedToPurple->Insert(ColorScaleElement(0.666, Qt::cyan));
	rainbowRedToPurple->Insert(ColorScaleElement(0.833, Qt::blue));
	result["Rainbow:Red->Purple"] = rainbowRedToPurple;

	//彩虹：紫->红
	ColorScale* rainbowPurpleToRed = new ColorScale("Rainbow:Purple->Red");
	rainbowPurpleToRed->Insert(ColorScaleElement(0.0, QColor(160, 32, 240)));
	rainbowPurpleToRed->Insert(ColorScaleElement(1.0, Qt::red));
	rainbowPurpleToRed->Insert(ColorScaleElement(0.166, Qt::blue));
	rainbowPurpleToRed->Insert(ColorScaleElement(0.333, Qt::cyan));
	rainbowPurpleToRed->Insert(ColorScaleElement(0.5, Qt::green));
	rainbowPurpleToRed->Insert(ColorScaleElement(0.666, Qt::yellow));
	rainbowPurpleToRed->Insert(ColorScaleElement(0.833, QColor(255, 165, 0)));
	result["Rainbow:Purple->Red"] = rainbowPurpleToRed;

	//BGYR
	ColorScale* bgyr = new ColorScale("Blue>Green>Yellow>Red");
	bgyr->Insert(ColorScaleElement(0.0,Qt::blue));
	bgyr->Insert(ColorScaleElement(1.0/3.0,Qt::green));
	bgyr->Insert(ColorScaleElement(2.0/3.0,Qt::yellow));
	bgyr->Insert(ColorScaleElement(1.0,Qt::red));
	result["Blue>Green>Yellow>Red"] = bgyr;

	//RYGB
	ColorScale* rygb = new ColorScale("Red>Yellow>Green>Blue");
	rygb->Insert(ColorScaleElement(0.0,Qt::red));
	rygb->Insert(ColorScaleElement(1.0/3.0,Qt::yellow));
	rygb->Insert(ColorScaleElement(2.0/3.0,Qt::green));
	rygb->Insert(ColorScaleElement(1.0,Qt::blue));
	result["Red>Yellow>Green>Blue"] = rygb;

	DefaultColorScales = result;

	return result;
}

QString ColorScale::GetName() const 
{ 
	return m_name;
}

void ColorScale::SetName(const QString& name) 
{
	m_name = name;
}

QString ColorScale::GetUniqueID() const 
{ 
	return m_uniqueID;
}


void ColorScale::SetUniqueID(unsigned uniqueID) 
{
	m_uniqueID = uniqueID;
}

bool ColorScale::IsRelative() const 
{ 
	return m_relative; 
}

void ColorScale::SetRelative() 
{
	m_relative = true;
}

bool ColorScale::IsLocked() const 
{
	return m_locked;
}

void ColorScale::SetLocked(bool state)
{
	m_locked = state;
}

int ColorScale::Count() const 
{
	return m_elements.size(); 
}

ColorScaleElement& ColorScale::Element(int index)
{
	return m_elements[index]; 
}

const ColorScaleElement& ColorScale::Element(int index) const
{
	return m_elements[index];
}

double ColorScale::GetRelativePosition(double value) const
{
	//assert(m_updated && !m_relative);
	return (value - m_absoluteMinValue)/m_absoluteRange;
}

 RGBAColor ColorScale::GetColorByValue(double value, const RGBAColor outOfRangeColor) 
{
//	assert(m_updated && !m_relative);
	double relativePos = GetRelativePosition(value);
	return (relativePos >= 0.0 && relativePos <= 1.0 ? GetColorByRelativePos(relativePos) : outOfRangeColor);
}

 RGBAColor ColorScale::GetColorByRelativePos(double relativePos) 
{
	assert(m_updated);
	if (relativePos >= 0.0 && relativePos <= 1.0)
	{
		auto aaa = relativePos * (double)(MAX_STEPS-1);
		return GetColorByIndex((unsigned)(relativePos * (double)(MAX_STEPS-1)));
	}
	else
	{
		RGBAColor outOfRangeColor = {0, 0, 0, 0};
		return outOfRangeColor;
	}
}

 RGBAColor ColorScale::GetColorByRelativePos(double relativePos, unsigned steps, const RGBAColor outOfRangeColor) 
{
	assert(m_updated);
	if (relativePos >= 0.0 && relativePos <= 1.0)
	{
		//quantized (16 bits) version --> much faster than floor!
		unsigned index = ((unsigned)((relativePos*(double)steps)*65535.0))>>16;
		return GetColorByIndex((index*(MAX_STEPS-1)) / (steps-1));
	}
	else
	{
		return outOfRangeColor;
	}
}

 RGBAColor ColorScale::GetColorByIndex(unsigned index) 
{
	assert(m_updated && index < MAX_STEPS);
	return m_rgbaScale[index];
}

//插入元素
void ColorScale::Insert(const ColorScaleElement& element, bool autoUpdate/*=true*/)
{
	//检验刻度尺是否被锁定
	if (m_locked)
	{
		Logger::Warning(QObject::tr("[ColorScale::insert] Scale '%1' is locked!").arg(m_name));
		return;
	}

	//添加元素
	m_elements.push_back(element);

	//
	m_updated = false;

	//进行更新刻度尺
	if (autoUpdate && m_elements.size() 
		>= (int)MIN_STEPS)
		Update();
}

//清空刻度尺
void ColorScale::Clear()
{
	//检验刻度尺是否被锁定
	if (m_locked)
	{
		Logger::Warning(QObject::tr("[ColorScale::clear] Scale '%1' is locked!").arg(m_name));
		return;
	}

	//清空元素
	m_elements.clear();

	m_updated = false;
}

//移除元素
void ColorScale::Remove(int index, bool autoUpdate/*=true*/)
{
	//检验刻度尺是否被锁定
	if (m_locked)
	{
		Logger::Warning(QObject::tr("[ColorScale::remove] Scale '%1' is locked!").arg(m_name));
		return;
	}

	m_elements.removeAt(index);
	m_updated = false;

	if (autoUpdate)
		Update();
}

//对元素进行排序
void ColorScale::Sort()
{
	qSort(m_elements.begin(), 
			m_elements.end(), 
			ColorScaleElement::IsSmaller);
}

//更新刻度尺
void ColorScale::Update()
{
	if (m_elements.size() 
		>= (int)MIN_STEPS)
	{
		//先进行排序
		Sort();

		unsigned stepCount = static_cast<unsigned>(m_elements.size());

		//判断值是否有效
		if (m_elements.front().Position() != 0.0 
			|| m_elements.back().Position() != 1.0)
		{
			//invalid scale: paint it black ;)
			RGBAColor color = {0, 0, 0};
			std::fill(m_rgbaScale.begin(), 
						m_rgbaScale.end(), color);

			//DcLib::DcLog::Warning(QObject::tr("[ColorScale] Scale '%1' is invalid! (boundaries are not [0.0-1.0]").arg(GetName()));
			return;
		}

		unsigned j = 0; //current intervale
		for (unsigned i = 0; i < MAX_STEPS; ++i)
		{
			double relativePos = static_cast<double>(i) / static_cast<double>(MAX_STEPS - 1);

			//forward to the right intervale
			while (j + 2 < stepCount && m_elements[j + 1].Position() < relativePos)
			{
				++j;
			}

			// 线性插值颜色
			DCVector3D colBefore(m_elements[j].Color().redF(),
								m_elements[j].Color().greenF(),
								m_elements[j].Color().blueF());

			DCVector3D colNext(m_elements[j+1].Color().redF(),
								m_elements[j+1].Color().greenF(),
								m_elements[j+1].Color().blueF());

			//透明度
			double alpha = (relativePos - m_elements[j].Position())
							/(m_elements[j+1].Position()
							- m_elements[j].Position());

			DCVector3D interpCol = colBefore + (colNext-colBefore) * alpha;

			m_rgbaScale[i][0] = static_cast<ColorType>(interpCol.x * (double)MAX_COLOR_COMP);
			m_rgbaScale[i][1] = static_cast<ColorType>(interpCol.y * (double)MAX_COLOR_COMP);
			m_rgbaScale[i][2] = static_cast<ColorType>(interpCol.z * (double)MAX_COLOR_COMP);
			m_rgbaScale[i][3] = MAX_COLOR_COMP; //do not dream: no transparency ;)
		}

		m_updated = true;
	}
	else //invalid scale: paint it black ;)
	{
		Logger::Warning(QObject::tr("[ColorScale] Scale '%1' is invalid! (not enough elements)").arg(GetName()));
		RGBAColor color = {0, 0, 0};
		std::fill(m_rgbaScale.begin(), m_rgbaScale.end(), color);
	}
}

void ColorScale::SetAbsolute(double minVal, double maxVal)
{
	assert(maxVal >= minVal);

	m_relative = false;

	m_absoluteMinValue = minVal;
	m_absoluteRange = maxVal - minVal;

	//as 'm_absoluteRange' is used for division, we make sure it is not left to 0!
	m_absoluteRange = std::max(m_absoluteRange, 1e-12);
}

void ColorScale::GetAbsoluteBoundaries(double& minVal, double& maxVal) const
{
	minVal = m_absoluteMinValue;
	maxVal = m_absoluteMinValue + m_absoluteRange;
}
