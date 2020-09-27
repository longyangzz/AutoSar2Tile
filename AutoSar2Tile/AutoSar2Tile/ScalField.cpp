#include "ScalField.h"

#include "ColorScale.h"

#include "GpBasicTypes.h"

ScalField::ScalField(QObject *parent)
	: QObject(parent)
	, m_colorSteps(50)
{
	m_name = "";
	m_colorScales = ColorScale::DefualtColorScales();
	m_currentColorScale = m_colorScales["Blue>Green>Yellow>Red"];

	
}

ScalField::ScalField(QString name)
    : m_colorSteps(50)
{
	m_name = name;
	m_colorScales = ColorScale::DefualtColorScales();
	m_currentColorScale = m_colorScales["Blue>Green>Yellow>Red"];
}

ScalField::~ScalField()
{
}

void ScalField::AddData(ScalarType data)
{
	m_data.push_back(data);
}
void ScalField::SetColorScale(ColorScale* colorScale)
{
	m_currentColorScale = colorScale;
}


ColorScale* ScalField::GetColorScale()
{
	return m_currentColorScale;
}


std::vector<RGBColor> ScalField::GetColors() const
{
	return m_colors;
}


//准备数据
void ScalField::Prepare()
{
	if (m_data.empty())
	{
		return;
	}
	auto interval = (*std::max_element(m_data.cbegin(), m_data.cend()) -
		*std::min_element(m_data.cbegin(), m_data.cend()));
	auto maxValue = *std::max_element(m_data.cbegin(), m_data.cend());
	auto minValue = *std::min_element(m_data.cbegin(), m_data.cend());

	//更新标量场最大最小值
	m_keyValues.clear();
	m_keyValues.insert(minValue);
	m_keyValues.insert(maxValue);

	m_colors.clear();

	for (auto i = m_data.begin(); i != m_data.end(); ++i)
	{
		RGBAColor outOfRangeColor = { 0, 0, 0, 0 };

		double rPos;
		if (interval == 0)
		{
			rPos = 1;
		}
		else
		{
			rPos = abs((*i - minValue) / interval);
		}
		RGBAColor rgbaColor = m_currentColorScale->GetColorByRelativePos(rPos, m_colorSteps, outOfRangeColor);
		RGBColor color = { rgbaColor[0], rgbaColor[1], rgbaColor[2] };
		m_colors.push_back(color);
	}
}

void ScalField::SetScaleFieldSize(long num)
{
	m_data.reserve(num);
}

//改变当前颜色表
void ScalField::ChangeColorScale(QString name)
{
	//如果颜色表不存在，则不进行操作
	if (m_colorScales.find(name) == m_colorScales.end())
	{
		return;
	}

	//改变当前颜色表
	m_currentColorScale = m_colorScales[name];
}

std::set<double> ScalField::GetScaleRange() const
{
	return m_keyValues;
}

RGBColor ScalField::GetColor(unsigned index)
{
	if (m_keyValues.empty())
	{
		Prepare();
	}

	return m_colors[index];
}

const RGBAColor ScalField::GetColor(ScalarType value)
{
	if (m_keyValues.empty())
	{
		Prepare();
	}
	std::vector<double> sortedKeyValues(m_keyValues.begin(), m_keyValues.end());
	double interval = sortedKeyValues.back() - sortedKeyValues.front();
	RGBAColor outOfRangeColor = { 0, 0, 0, 0 };
	return m_currentColorScale->GetColorByRelativePos(abs((value - sortedKeyValues.front()) / interval), m_colorSteps, outOfRangeColor);
}

QString ScalField::GetName() const
{
	return m_name;
}

void ScalField::ComputeMinAndMax()
{
	if (!m_borderValid)
	{
		if (!m_data.empty())
		{
			bool minMaxInitialized = false;
			for (unsigned i = 0; i < m_data.size(); ++i)
			{
				const ScalarType& val = m_data[i];
				if (minMaxInitialized)
				{
					if (val < m_minVal)
						m_minVal = val;
					else if (val > m_maxVal)
						m_maxVal = val;
				}
				else
				{
					//first valid value is used to init min and max
					m_minVal = m_maxVal = val;
					minMaxInitialized = true;
				}
			}
		}
		else
		{
			m_minVal = m_maxVal = 0;
		}
		m_borderValid = true;
	}
}


ScalarType ScalField::GetMin()
{
	ComputeMinAndMax();
	return m_minVal;
}

ScalarType ScalField::GetMax()
{
	ComputeMinAndMax();
	return m_maxVal;
}

void ScalField::AddData(unsigned index, ScalarType data)
{
	m_data[index] = data;
}

double ScalField::GetPointScalarValue(unsigned index)
{
	return m_data.at(index);
}

unsigned ScalField::Size()
{
	return m_data.size();
}

void ScalField::SetDatas(const unsigned& size, const int& value)
{
	//填充容器
	m_data.resize(size);
	std::fill(m_data.begin(), m_data.end(), value);
}

void ScalField::SetDatas(std::vector<ScalarType> values)
{
	if (m_data.size() > 0)
	{
		m_data = values;
	}
}

std::vector<ScalarType >& ScalField::GetDatas()
{
	return m_data;
}

void ScalField::SetColorSteps(unsigned steps)
{
	if (steps > 1024)
		m_colorSteps = 1024;
	else if (steps < 2)
		m_colorSteps = 2;
	else
		m_colorSteps = steps;
}