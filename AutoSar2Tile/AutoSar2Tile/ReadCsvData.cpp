#include "ReadCsvData.h"

//���캯������ʼ���ļ�·��
CSVreader::CSVreader(const char *path)
{
	_csvInput.open(path);
}

CSVreader::~CSVreader()
{
	_csvInput.close();
}
//��ȡcsv�ļ�����
int  CSVreader::readline()
{
	//����һ������Ϊ�ַ���
	string _Oneline;
	//��ȡһ������
	getline(_csvInput, _Oneline);

	//cout << "ԭʼ�ַ�����" << _Oneline << endl;

	//�����ȡ���������ݣ�����ʧ��
	if (_Oneline[0] == 'P')
		return EXIT_FAILURE;
	//�����ַ���������
	istringstream _Readstr(_Oneline);
	//����һ�������еĸ����ִ�
	string _partOfstr;
	//��һ�����ݰ�'��'�ָ�
	for (int i = 0; i < 10; i++)
	{
		getline(_Readstr, _partOfstr, ',');
		data[i] = atof(_partOfstr.c_str());

	}
	//�ж��Ƿ��������
	if ((data[4] || data[5] || data[6]) == 0)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
