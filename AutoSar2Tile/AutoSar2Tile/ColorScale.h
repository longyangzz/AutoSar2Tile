#ifndef DCGP_COLOR_SCALE_H
#define DCGP_COLOR_SCALE_H

//C++标准库
#include <assert.h>
#include <array>
#include <map>

#include "QColor"

//Qt标准库
#include "QList"
class QString;
class QColor;

//Local
#include "GpBasicTypes.h"

class ColorScaleElement
{
public:
	//默认构造函数
	ColorScaleElement();
	//根据位置设置颜色
	ColorScaleElement(double position, QColor color);

	//关联位置
	inline double Position() const;
	inline double& Position();

	//获取颜色
	inline QColor Color() const;
	inline QColor& Color();

	//比较两个颜色元素
	inline static bool IsSmaller(const ColorScaleElement& e1, const ColorScaleElement& e2);

protected:


private:
	//刻度尺中的位置
	//必须在0.0~1.0之间
	double m_position;
	//颜色
	QColor m_color;
};

class ColorScale
{
public:
	//默认构造函数
	//参数一：刻度尺的名字
	//参数二：Unique ID
	ColorScale(QString name);

	//使用颜色元素进行构造
	ColorScale(QString name, QList<ColorScaleElement> elements);

	//析构函数
	virtual ~ColorScale();

	//获取默认的原色表
	static std::map<QString, ColorScale*> DefualtColorScales();

	//获取名字
	QString GetName() const;

	//设置名字
	void SetName(const QString& name);

	//返回唯一标识
	QString GetUniqueID() const;

	//设置唯一标记
	void SetUniqueID(unsigned uniqueID);
	inline bool IsRelative() const;

	inline void SetRelative();

	void SetAbsolute(double minVal, double maxVal);

	void GetAbsoluteBoundaries(double& minVal, double& maxVal) const;

	//刻度尺是否锁定，锁定则不可更改
	inline bool IsLocked() const;

	//设置刻度尺是否锁定
	inline void SetLocked(bool state);

	//返回当前的步长
	inline int Count() const;

	//获取指定位置的元素
	inline ColorScaleElement& Element(int index);

	//获取指定位置的元素(const)
	inline const ColorScaleElement& Element(int index) const;

	//添加元素，刻度尺可操作状态下
	void Insert(const ColorScaleElement& step, bool autoUpdate = true);

	//删除指定的元素
	void Remove(int index, bool autoUpdate = true);

	//清除所有元素
	void Clear();

	//更新
	void Update();

	inline double GetRelativePosition(double value) const;

	 RGBAColor GetColorByValue(double value, const RGBAColor outOfRangeColor);

	RGBAColor GetColorByRelativePos(double relativePos);

	 RGBAColor GetColorByRelativePos(double relativePos, unsigned steps, const RGBAColor outOfRangeColor);

	//通过索引获取颜色
	  RGBAColor GetColorByIndex(unsigned index) ;

protected:
	ColorScale();
	//对元素进行排序
	void Sort();

private:
	//计数
	static unsigned s_number;
	//最小步长
	static const unsigned MIN_STEPS = 2;
	//默认步长
	static const unsigned DEFAULT_STEPS = 256;
	//最大步长
	static const unsigned MAX_STEPS = 1024;
	//名字
	QString m_name;
	//唯一标识
	unsigned m_uniqueID;
	//元素
	QList<ColorScaleElement> m_elements;
	//
	std::array<RGBAColor, MAX_STEPS> m_rgbaScale;
	//
	bool m_updated;
	//
	bool m_relative;
	//是否被锁定
	bool m_locked;
	double m_absoluteMinValue;
	double m_absoluteRange;
};
#endif //CC_COLOR_SCALE_HEADER
