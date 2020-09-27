#pragma once

#include "GpBasicTypes.h"
#include <QObject>

#include <set>

class ColorScale;
class ScalField : public QObject
{
	Q_OBJECT

public:
	ScalField(QObject *parent);
	ScalField(QString name);
	~ScalField();

	class ScalarRange
	{
	public:
		//构造函数
		ScalarRange()
			: m_min(0)
			, m_max(0)
		{}

		inline ScalarType Range() const { return m_max - m_min; }

	private:
		//最大值
		ScalarType m_max;
		//最小值
		ScalarType m_min;
	};

	//添加数据
	void AddData(ScalarType data);

	//添加数据到指定的索引位置
	void AddData(unsigned index, ScalarType data);

	//设置颜色表
	inline void SetColorScale(ColorScale* colorScale);

	//获取颜色表
	ColorScale* GetColorScale();

	//获取颜色
	std::vector<RGBColor> GetColors() const;

	//对数据进行预处理，算出其数值范围
	void Prepare();

	//!\brief 获取显示范围
	//!\retval 显示范围
	std::set<double> GetScaleRange() const;

	//!\brief 获取指定位置的颜色值
	const RGBAColor GetColor(ScalarType value);

	//!\brief 获取指定索引号的颜色值
	RGBColor GetColor(unsigned index);

	//!\brief 获取当前标量的名字
	//!\retval 名字
	QString GetName() const;

	void ComputeMinAndMax();


	ScalarType GetMin();

	ScalarType GetMax();

	double GetPointScalarValue(unsigned index);

	unsigned Size();

	//设置更新颜色数
	void SetColorSteps(unsigned steps);

	//设置m_data的大小
	void SetScaleFieldSize(long num);
	//改变颜色表
	void ChangeColorScale(QString name);

	//***********************为las数据处理添加的方法**************************************************
	void SetDatas(const unsigned& size, const int& value);
	void SetDatas(std::vector<ScalarType> values);
	std::vector<ScalarType>& GetDatas();
protected:

	//!\brief 显示范围
	ScalarRange m_displayRange;

	//名字
	QString m_name;

	//所有颜色表
	std::map<QString, ColorScale*> m_colorScales;

	//当前颜色表
	ColorScale* m_currentColorScale;

	//标量值
	std::vector<ScalarType> m_data;

	//每个标量对应的颜色
	std::vector<RGBColor> m_colors;

	//变量场的最大最小值
	std::set<double> m_keyValues;

	//m_data中的最大最小值
	ScalarType m_minVal;

	//m_data中的最大最小值
	ScalarType m_maxVal;

	//是否已经计算过最大最小值
	bool m_borderValid;

	//颜色个数
	unsigned m_colorSteps;
};
