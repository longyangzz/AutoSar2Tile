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
		//���캯��
		ScalarRange()
			: m_min(0)
			, m_max(0)
		{}

		inline ScalarType Range() const { return m_max - m_min; }

	private:
		//���ֵ
		ScalarType m_max;
		//��Сֵ
		ScalarType m_min;
	};

	//�������
	void AddData(ScalarType data);

	//������ݵ�ָ��������λ��
	void AddData(unsigned index, ScalarType data);

	//������ɫ��
	inline void SetColorScale(ColorScale* colorScale);

	//��ȡ��ɫ��
	ColorScale* GetColorScale();

	//��ȡ��ɫ
	std::vector<RGBColor> GetColors() const;

	//�����ݽ���Ԥ�����������ֵ��Χ
	void Prepare();

	//!\brief ��ȡ��ʾ��Χ
	//!\retval ��ʾ��Χ
	std::set<double> GetScaleRange() const;

	//!\brief ��ȡָ��λ�õ���ɫֵ
	const RGBAColor GetColor(ScalarType value);

	//!\brief ��ȡָ�������ŵ���ɫֵ
	RGBColor GetColor(unsigned index);

	//!\brief ��ȡ��ǰ����������
	//!\retval ����
	QString GetName() const;

	void ComputeMinAndMax();


	ScalarType GetMin();

	ScalarType GetMax();

	double GetPointScalarValue(unsigned index);

	unsigned Size();

	//���ø�����ɫ��
	void SetColorSteps(unsigned steps);

	//����m_data�Ĵ�С
	void SetScaleFieldSize(long num);
	//�ı���ɫ��
	void ChangeColorScale(QString name);

	//***********************Ϊlas���ݴ�����ӵķ���**************************************************
	void SetDatas(const unsigned& size, const int& value);
	void SetDatas(std::vector<ScalarType> values);
	std::vector<ScalarType>& GetDatas();
protected:

	//!\brief ��ʾ��Χ
	ScalarRange m_displayRange;

	//����
	QString m_name;

	//������ɫ��
	std::map<QString, ColorScale*> m_colorScales;

	//��ǰ��ɫ��
	ColorScale* m_currentColorScale;

	//����ֵ
	std::vector<ScalarType> m_data;

	//ÿ��������Ӧ����ɫ
	std::vector<RGBColor> m_colors;

	//�������������Сֵ
	std::set<double> m_keyValues;

	//m_data�е������Сֵ
	ScalarType m_minVal;

	//m_data�е������Сֵ
	ScalarType m_maxVal;

	//�Ƿ��Ѿ�����������Сֵ
	bool m_borderValid;

	//��ɫ����
	unsigned m_colorSteps;
};
