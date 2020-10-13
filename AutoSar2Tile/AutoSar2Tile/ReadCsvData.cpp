#include "ReadCsvData.h"

//构造函数，初始化文件路径
CSVreader::CSVreader(const char *path)
{
	_csvInput.open(path);
}

CSVreader::~CSVreader()
{
	_csvInput.close();
}
//读取csv文件数据
int  CSVreader::readline()
{
	//定义一行数据为字符串
	string _Oneline;
	//读取一行数据
	getline(_csvInput, _Oneline);

	//cout << "原始字符串：" << _Oneline << endl;

	//如果读取到首行数据，返回失败
	if (_Oneline[0] == 'P')
		return EXIT_FAILURE;
	//定义字符串流对象
	istringstream _Readstr(_Oneline);
	//定义一行数据中的各个字串
	string _partOfstr;
	//将一行数据按'，'分割
	for (int i = 0; i < 10; i++)
	{
		getline(_Readstr, _partOfstr, ',');
		data[i] = atof(_partOfstr.c_str());

	}
	//判断是否读完数据
	if ((data[4] || data[5] || data[6]) == 0)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
