#ifndef DCGP_COLOR_SCALE_H
#define DCGP_COLOR_SCALE_H

//C++��׼��
#include <assert.h>
#include <array>
#include <map>

#include "QColor"

//Qt��׼��
#include "QList"
class QString;
class QColor;

//Local
#include "GpBasicTypes.h"

class ColorScaleElement
{
public:
	//Ĭ�Ϲ��캯��
	ColorScaleElement();
	//����λ��������ɫ
	ColorScaleElement(double position, QColor color);

	//����λ��
	inline double Position() const;
	inline double& Position();

	//��ȡ��ɫ
	inline QColor Color() const;
	inline QColor& Color();

	//�Ƚ�������ɫԪ��
	inline static bool IsSmaller(const ColorScaleElement& e1, const ColorScaleElement& e2);

protected:


private:
	//�̶ȳ��е�λ��
	//������0.0~1.0֮��
	double m_position;
	//��ɫ
	QColor m_color;
};

class ColorScale
{
public:
	//Ĭ�Ϲ��캯��
	//����һ���̶ȳߵ�����
	//��������Unique ID
	ColorScale(QString name);

	//ʹ����ɫԪ�ؽ��й���
	ColorScale(QString name, QList<ColorScaleElement> elements);

	//��������
	virtual ~ColorScale();

	//��ȡĬ�ϵ�ԭɫ��
	static std::map<QString, ColorScale*> DefualtColorScales();

	//��ȡ����
	QString GetName() const;

	//��������
	void SetName(const QString& name);

	//����Ψһ��ʶ
	QString GetUniqueID() const;

	//����Ψһ���
	void SetUniqueID(unsigned uniqueID);
	inline bool IsRelative() const;

	inline void SetRelative();

	void SetAbsolute(double minVal, double maxVal);

	void GetAbsoluteBoundaries(double& minVal, double& maxVal) const;

	//�̶ȳ��Ƿ������������򲻿ɸ���
	inline bool IsLocked() const;

	//���ÿ̶ȳ��Ƿ�����
	inline void SetLocked(bool state);

	//���ص�ǰ�Ĳ���
	inline int Count() const;

	//��ȡָ��λ�õ�Ԫ��
	inline ColorScaleElement& Element(int index);

	//��ȡָ��λ�õ�Ԫ��(const)
	inline const ColorScaleElement& Element(int index) const;

	//���Ԫ�أ��̶ȳ߿ɲ���״̬��
	void Insert(const ColorScaleElement& step, bool autoUpdate = true);

	//ɾ��ָ����Ԫ��
	void Remove(int index, bool autoUpdate = true);

	//�������Ԫ��
	void Clear();

	//����
	void Update();

	inline double GetRelativePosition(double value) const;

	 RGBAColor GetColorByValue(double value, const RGBAColor outOfRangeColor);

	RGBAColor GetColorByRelativePos(double relativePos);

	 RGBAColor GetColorByRelativePos(double relativePos, unsigned steps, const RGBAColor outOfRangeColor);

	//ͨ��������ȡ��ɫ
	  RGBAColor GetColorByIndex(unsigned index) ;

protected:
	ColorScale();
	//��Ԫ�ؽ�������
	void Sort();

private:
	//����
	static unsigned s_number;
	//��С����
	static const unsigned MIN_STEPS = 2;
	//Ĭ�ϲ���
	static const unsigned DEFAULT_STEPS = 256;
	//��󲽳�
	static const unsigned MAX_STEPS = 1024;
	//����
	QString m_name;
	//Ψһ��ʶ
	unsigned m_uniqueID;
	//Ԫ��
	QList<ColorScaleElement> m_elements;
	//
	std::array<RGBAColor, MAX_STEPS> m_rgbaScale;
	//
	bool m_updated;
	//
	bool m_relative;
	//�Ƿ�����
	bool m_locked;
	double m_absoluteMinValue;
	double m_absoluteRange;
};
#endif //CC_COLOR_SCALE_HEADER
