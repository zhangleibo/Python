/************************************************************************/
/*                  C++ �е���python�ṩ����                             */
/************************************************************************/
#include "Python.h"
#include <iostream>
using namespace std;

/*
 *1.���python.h ��װĿ¼ ����C:\Python27\include
 *2.lib��·�� ����C:\Python27\libs 
 *	debug��python27_d.lib release��python27.lib
 *
 *
 */
//��c++�е�python����ʱ��������������⣬pythonÿ���汾�����������ơ�
//
//��vs������α��ġ�debug�汾�����ܳ�����������
//
//�����Լ�α���debug��Python27_d.lib��Python27_d.dll���ܳ��ֵ����⣺
//
//��Ҫ����Ϊ Py_DEBUG / Py_TRACE_REFS ����, �޸� Python\include �µ� pyconfig.h, object.h
//
//�������
//
//�޸� pyconfig.h
//�޸�
//
//#ifdef _DEBUG 
//# define Py_DEBUG 
//#endif 
//
//Ϊ
//
//#ifdef _DEBUG 
////# define Py_DEBUG 
//#endif 
//
//�޸�
//
//# ifdef _DEBUG 
//# pragma comment(lib,"python27_d.lib") 
//# else 
//# pragma comment(lib,"python27.lib") 
//# endif /* _DEBUG */ 
//
//Ϊ
//
//# ifdef _DEBUG 
//# pragma comment(lib,"python27.lib") 
//# else 
//# pragma comment(lib,"python27.lib") 
//# endif /* _DEBUG */ 
//
//�޸�object.h
//�޸�
//#if defined(Py_DEBUG) && !defined(Py_TRACE_REFS) 
//#define Py_TRACE_REFS 
//#endif 
//
//Ϊ
//
//#if defined(Py_DEBUG) && !defined(Py_TRACE_REFS) 
//// #define Py_TRACE_REFS 
//#endif


#pragma comment(lib, "python27.lib")


 #include <iostream>  
 #include <Python.h>  
 
 using namespace std;
 
 void HelloWorld();
 void Add();
 void TestTransferDict();
 void TestClass();
 int test_demo();

// #test.py
//  def HelloWorld() :
// 	 print "Hello World"
// 	 def add(a, b) :
// 	 return a + b
// 	 def TestDict(dict) :
// 	 print dict
// 	 dict["Age"] = 17
// 	 return dict
//  class Person :
// 	 def greet(self, greetStr) :
// 	 print greetStr
// #print add(5, 7)
// #a = raw_input("Enter To Continue...")

 int main()
 {
 	cout << "Starting Test..." << endl;
 
 	cout << "HelloWorld()-------------" << endl;
 	HelloWorld();
 	cout << "Add()--------------------" << endl;
 	Add();
 	cout << "TestDict-----------------" << endl;
 	TestTransferDict();
 	cout << "TestClass----------------" << endl;
 	TestClass();
 
	test_demo();
 	system("pause");
 	return 0;
 }
 
 //�������"Hello World"����  
 void HelloWorld()
 {
	//////////////////////////////////////////////////////////////////////////
	//��ʼ��
	//ʹ��python֮ǰ��Ҫ����Py_Initialize();����������г�ʼ��  
	Py_Initialize();

	// ����ʼ���Ƿ�ɹ�
	if (!Py_IsInitialized()) {
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	//����ģ��·��(.py�ļ�·��) Ĭ����.exe����Ŀ¼�²����ļ����ɲ�������·��

	//test001.py�ļ����xxx.exe�ļ����·��
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("print '---import sys---'");
	PyRun_SimpleString("sys.path.append('../')");

	//////////////////////////////////////////////////////////////////////////
	//ģ�����
	//������Ҫ���õ�test001ΪPython�ļ���
	PyObject * pModule = PyImport_ImportModule("test001");
	//������֤ģ��
	if (NULL == pModule) {
		cout << "[ERROR] Python get module failed." << endl;
		return;
	}
	else
	{
		cout << "[INFO] Python get module succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	//��������
	//������Ҫ���õĺ�����
	PyObject * pFunc = PyObject_GetAttrString(pModule, "HelloWorld");
	// ��֤�����Ƿ���سɹ�
	if (NULL == pFunc || !PyCallable_Check(pFunc))  
	{
		cout << "[ERROR] Can't find funftion (HelloWorld)" << endl;
		return ;
	}
	else
	{
		cout << "[INFO] Get function (HelloWorld) succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	//��������
	//���ú���,NULL��ʾ����Ϊ�� 
	PyObject* pReturn = PyEval_CallObject(pFunc, NULL);
	if (NULL == pReturn)
	{
		cout << "[Error] Call function (HelloWorld) failed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	//�ͷ���Դ
	//����Py_Finalize,�����Py_Initialize���Ӧ��.
	Py_Finalize();
}

//����Add����,������int�Ͳ���  
void Add()
{
	//////////////////////////////////////////////////////////////////////////
	Py_Initialize();

	// ����ʼ���Ƿ�ɹ�
	if (!Py_IsInitialized()) {
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//����ģ��·��(.py�ļ�·��) Ĭ����.exe����Ŀ¼�²����ļ����ɲ�������·��

	//test001.py�ļ����xxx.exe�ļ����·��
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("print '---import sys---'");
	PyRun_SimpleString("sys.path.append('../')");

	//////////////////////////////////////////////////////////////////////////
	PyObject * pModule = PyImport_ImportModule("test001");      //test001:Python�ļ���
	//������֤ģ��
	if (NULL == pModule) {
		cout << "[ERROR] Python get module failed." << endl;
		return;
	}
	else
	{
		cout << "[INFO] Python get module succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	PyObject * pFunc = PyObject_GetAttrString(pModule, "add");  //Add:Python�ļ��еĺ�����  
	// ��֤�����Ƿ���سɹ�
	if (NULL == pFunc || !PyCallable_Check(pFunc))
	{
		cout << "[ERROR] Can't find funftion (add)" << endl;
		return;
	}
	else
	{
		cout << "[INFO] Get function (add) succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	//��������:  
	PyObject *pArgs = PyTuple_New(2);               //�������õĲ������ݾ�����Ԫ�����ʽ�����,2��ʾ��������  
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 5));//0---���  i��ʾ����int�ͱ���  
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 7));//1---���  

	//////////////////////////////////////////////////////////////////////////
	//����ֵ  
	PyObject *pReturn = PyEval_CallObject(pFunc, pArgs);      //���ú���

	if (pReturn)
	{
		//������ֵת��Ϊint����  
		int result;
		PyArg_Parse(pReturn, "i", &result);    //i��ʾת����int�ͱ���  

		//or 
		result = PyInt_AsLong(pReturn);
		cout << "5+7 = " << result << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	Py_Finalize();
}

//�������ݵ�����Ϊ�ֵ�  
void TestTransferDict()
{
	//////////////////////////////////////////////////////////////////////////
	Py_Initialize();

	// ����ʼ���Ƿ�ɹ�
	if (!Py_IsInitialized()) {
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//����ģ��·��(.py�ļ�·��) Ĭ����.exe����Ŀ¼�²����ļ����ɲ�������·��

	//test001.py�ļ����xxx.exe�ļ����·��
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("print '---import sys---'");
	PyRun_SimpleString("sys.path.append('../')");

	//////////////////////////////////////////////////////////////////////////
	PyObject * pModule = PyImport_ImportModule("test001");      //test001:Python�ļ���  
	//������֤ģ��
	if (NULL == pModule) {
		cout << "[ERROR] Python get module failed." << endl;
		return;
	}
	else
	{
		cout << "[INFO] Python get module succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	PyObject * pFunc = PyObject_GetAttrString(pModule, "TestDict"); //TestDict:Python�ļ��еĺ�����  
	// ��֤�����Ƿ���سɹ�
	if (NULL == pFunc || !PyCallable_Check(pFunc))
	{
		cout << "[ERROR] Can't find funftion (TestDict)" << endl;
		return;
	}
	else
	{
		cout << "[INFO] Get function (TestDict) succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	//��������:  
	PyObject *pArgs = PyTuple_New(1);
	PyObject *pDict = PyDict_New();   //�����ֵ����ͱ���  

	PyDict_SetItemString(pDict, "Name", Py_BuildValue("s", "zlb")); //���ֵ����ͱ������������  
	PyDict_SetItemString(pDict, "Age", Py_BuildValue("i", 25));         //���ֵ����ͱ������������  
	PyTuple_SetItem(pArgs, 0, pDict);//0---���  ���ֵ����ͱ�����ӵ�����Ԫ����  


	//����ֵ  
	PyObject *pReturn = PyEval_CallObject(pFunc, pArgs);      //���ú���  

	//////////////////////////////////////////////////////////////////////////
	if (pReturn)
	{
		//������ֵ:  
		int size = PyDict_Size(pReturn);
		cout << "�����ֵ�Ĵ�СΪ: " << size << endl;

		PyObject *pNewAge = PyDict_GetItemString(pReturn, "Age");

		int newAge;
		PyArg_Parse(pNewAge, "i", &newAge);
		cout << "True Age: " << newAge << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	Py_Finalize();
}

//������  
void TestClass()
{
	//////////////////////////////////////////////////////////////////////////
	Py_Initialize();

	// ����ʼ���Ƿ�ɹ�
	if (!Py_IsInitialized()) {
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//����ģ��·��(.py�ļ�·��) Ĭ����.exe����Ŀ¼�²����ļ����ɲ�������·��

	//test001.py�ļ����xxx.exe�ļ����·��
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("print '---import sys---'");
	PyRun_SimpleString("sys.path.append('../')");

	//////////////////////////////////////////////////////////////////////////
	PyObject * pModule = PyImport_ImportModule("test001");      //test001:Python�ļ���  
	//������֤ģ��
	if (NULL == pModule) {
		cout << "[ERROR] Python get module failed." << endl;
		return;
	}
	else
	{
		cout << "[INFO] Python get module succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	//��ȡPerson��  
	PyObject *pClassPerson = PyObject_GetAttrString(pModule, "Person");//Person:Python�ļ��е�����  
	// ��֤�����Ƿ���سɹ�
	if (NULL == pClassPerson || !PyCallable_Check(pClassPerson))
	{
		cout << "[ERROR] Can't find funftion (pClassPerson)" << endl;
		return;
	}
	else
	{
		cout << "[INFO] Get function (pClassPerson) succeed." << endl;
	}

	//����Person���ʵ��  
	PyObject *pInstancePerson = PyInstance_New(pClassPerson, NULL, NULL);
	if (NULL != pInstancePerson)
	{
		//���÷���  
		PyObject *pReturn = PyObject_CallMethod(pInstancePerson, "greet", "s", "Hello Kitty");   //s��ʾ���ݵ����ַ���,ֵΪ"Hello Kitty"
		if (NULL == pReturn)
		{
			cout << "[Error] Call function (pClassPerson) failed." << endl;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Py_Finalize();
}


int test_demo()
{
	Py_Initialize();    // ��ʼ��

	// ��Python����·���л���������ģ������Ŀ¼��һ��Ҫ��֤·��������ȷ��
	string path = "../";

	string chdir_cmd = string("sys.path.append(\"") + path + "\")";//���Ϊ"sys.path.append(\"./testxxx.py")"
	const char* cstr_cmd = chdir_cmd.c_str();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString(cstr_cmd);

	// ����ģ��
	PyObject* moduleName = PyString_FromString("test001"); //�ļ��� �磺test001.py
	PyObject* pModule = PyImport_Import(moduleName);
	if (!pModule) // ����ģ��ʧ��
	{
	 cout << "[ERROR] Python get module failed." << endl;
	 return 0;
	}
	cout << "[INFO] Python get module succeed." << endl;

	// ���غ���
	PyObject* pv = PyObject_GetAttrString(pModule, "add");
	if (!pv || !PyCallable_Check(pv))  // ��֤�Ƿ���سɹ�
	{
	 cout << "[ERROR] Can't find funftion (add)" << endl;
	 return 0;
	}

	cout << "[INFO] Get function (add) succeed." << endl;

	// ���ò���
	PyObject* args = PyTuple_New(2);   // 2������
	PyObject* arg1 = PyInt_FromLong(4);    // ����һ��Ϊ4
	PyObject* arg2 = PyInt_FromLong(3);    // ��������Ϊ3
	PyTuple_SetItem(args, 0, arg1);
	PyTuple_SetItem(args, 1, arg2);

	// ���ú���
	PyObject* pRet = PyObject_CallObject(pv, args);

	// ��ȡ����
	if (pRet)  // ��֤�Ƿ���óɹ�
	{
	 long result = PyInt_AsLong(pRet);
	 cout << "result:" << result;
	}

	Py_Finalize();      //�ͷ���Դ

	return 0;
}