/************************************************************************/
/*                  C++ 中调用python提供代码                             */
/************************************************************************/
#include "Python.h"
#include <iostream>
using namespace std;

/*
 *1.添加python.h 安装目录 例如C:\Python27\include
 *2.lib库路径 例如C:\Python27\libs 
 *	debug：python27_d.lib release：python27.lib
 *
 *
 */
//在c++中调python函数时，出现了这个问题，python每个版本额解决方法类似。
//
//在vs下引用伪造的”debug版本“可能出现链接问题
//
//引用自己伪造的debug版Python27_d.lib、Python27_d.dll可能出现的问题：
//
//主要是因为 Py_DEBUG / Py_TRACE_REFS 引起, 修改 Python\include 下的 pyconfig.h, object.h
//
//解决方法
//
//修改 pyconfig.h
//修改
//
//#ifdef _DEBUG 
//# define Py_DEBUG 
//#endif 
//
//为
//
//#ifdef _DEBUG 
////# define Py_DEBUG 
//#endif 
//
//修改
//
//# ifdef _DEBUG 
//# pragma comment(lib,"python27_d.lib") 
//# else 
//# pragma comment(lib,"python27.lib") 
//# endif /* _DEBUG */ 
//
//为
//
//# ifdef _DEBUG 
//# pragma comment(lib,"python27.lib") 
//# else 
//# pragma comment(lib,"python27.lib") 
//# endif /* _DEBUG */ 
//
//修改object.h
//修改
//#if defined(Py_DEBUG) && !defined(Py_TRACE_REFS) 
//#define Py_TRACE_REFS 
//#endif 
//
//为
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
 
 //调用输出"Hello World"函数  
 void HelloWorld()
 {
	//////////////////////////////////////////////////////////////////////////
	//初始化
	//使用python之前，要调用Py_Initialize();这个函数进行初始化  
	Py_Initialize();

	// 检查初始化是否成功
	if (!Py_IsInitialized()) {
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	//加载模块路径(.py文件路径) 默认在.exe运行目录下查找文件，可不用设置路径

	//test001.py文件相对xxx.exe文件相对路径
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("print '---import sys---'");
	PyRun_SimpleString("sys.path.append('../')");

	//////////////////////////////////////////////////////////////////////////
	//模块加载
	//这里是要调用的test001为Python文件名
	PyObject * pModule = PyImport_ImportModule("test001");
	//加载验证模块
	if (NULL == pModule) {
		cout << "[ERROR] Python get module failed." << endl;
		return;
	}
	else
	{
		cout << "[INFO] Python get module succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	//函数加载
	//这里是要调用的函数名
	PyObject * pFunc = PyObject_GetAttrString(pModule, "HelloWorld");
	// 验证函数是否加载成功
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
	//函数调用
	//调用函数,NULL表示参数为空 
	PyObject* pReturn = PyEval_CallObject(pFunc, NULL);
	if (NULL == pReturn)
	{
		cout << "[Error] Call function (HelloWorld) failed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	//释放资源
	//调用Py_Finalize,这个和Py_Initialize相对应的.
	Py_Finalize();
}

//调用Add函数,传两个int型参数  
void Add()
{
	//////////////////////////////////////////////////////////////////////////
	Py_Initialize();

	// 检查初始化是否成功
	if (!Py_IsInitialized()) {
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//加载模块路径(.py文件路径) 默认在.exe运行目录下查找文件，可不用设置路径

	//test001.py文件相对xxx.exe文件相对路径
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("print '---import sys---'");
	PyRun_SimpleString("sys.path.append('../')");

	//////////////////////////////////////////////////////////////////////////
	PyObject * pModule = PyImport_ImportModule("test001");      //test001:Python文件名
	//加载验证模块
	if (NULL == pModule) {
		cout << "[ERROR] Python get module failed." << endl;
		return;
	}
	else
	{
		cout << "[INFO] Python get module succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	PyObject * pFunc = PyObject_GetAttrString(pModule, "add");  //Add:Python文件中的函数名  
	// 验证函数是否加载成功
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
	//创建参数:  
	PyObject *pArgs = PyTuple_New(2);               //函数调用的参数传递均是以元组的形式打包的,2表示参数个数  
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 5));//0---序号  i表示创建int型变量  
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 7));//1---序号  

	//////////////////////////////////////////////////////////////////////////
	//返回值  
	PyObject *pReturn = PyEval_CallObject(pFunc, pArgs);      //调用函数

	if (pReturn)
	{
		//将返回值转换为int类型  
		int result;
		PyArg_Parse(pReturn, "i", &result);    //i表示转换成int型变量  

		//or 
		result = PyInt_AsLong(pReturn);
		cout << "5+7 = " << result << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	Py_Finalize();
}

//参数传递的类型为字典  
void TestTransferDict()
{
	//////////////////////////////////////////////////////////////////////////
	Py_Initialize();

	// 检查初始化是否成功
	if (!Py_IsInitialized()) {
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//加载模块路径(.py文件路径) 默认在.exe运行目录下查找文件，可不用设置路径

	//test001.py文件相对xxx.exe文件相对路径
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("print '---import sys---'");
	PyRun_SimpleString("sys.path.append('../')");

	//////////////////////////////////////////////////////////////////////////
	PyObject * pModule = PyImport_ImportModule("test001");      //test001:Python文件名  
	//加载验证模块
	if (NULL == pModule) {
		cout << "[ERROR] Python get module failed." << endl;
		return;
	}
	else
	{
		cout << "[INFO] Python get module succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	PyObject * pFunc = PyObject_GetAttrString(pModule, "TestDict"); //TestDict:Python文件中的函数名  
	// 验证函数是否加载成功
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
	//创建参数:  
	PyObject *pArgs = PyTuple_New(1);
	PyObject *pDict = PyDict_New();   //创建字典类型变量  

	PyDict_SetItemString(pDict, "Name", Py_BuildValue("s", "zlb")); //往字典类型变量中填充数据  
	PyDict_SetItemString(pDict, "Age", Py_BuildValue("i", 25));         //往字典类型变量中填充数据  
	PyTuple_SetItem(pArgs, 0, pDict);//0---序号  将字典类型变量添加到参数元组中  


	//返回值  
	PyObject *pReturn = PyEval_CallObject(pFunc, pArgs);      //调用函数  

	//////////////////////////////////////////////////////////////////////////
	if (pReturn)
	{
		//处理返回值:  
		int size = PyDict_Size(pReturn);
		cout << "返回字典的大小为: " << size << endl;

		PyObject *pNewAge = PyDict_GetItemString(pReturn, "Age");

		int newAge;
		PyArg_Parse(pNewAge, "i", &newAge);
		cout << "True Age: " << newAge << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	Py_Finalize();
}

//测试类  
void TestClass()
{
	//////////////////////////////////////////////////////////////////////////
	Py_Initialize();

	// 检查初始化是否成功
	if (!Py_IsInitialized()) {
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//加载模块路径(.py文件路径) 默认在.exe运行目录下查找文件，可不用设置路径

	//test001.py文件相对xxx.exe文件相对路径
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("print '---import sys---'");
	PyRun_SimpleString("sys.path.append('../')");

	//////////////////////////////////////////////////////////////////////////
	PyObject * pModule = PyImport_ImportModule("test001");      //test001:Python文件名  
	//加载验证模块
	if (NULL == pModule) {
		cout << "[ERROR] Python get module failed." << endl;
		return;
	}
	else
	{
		cout << "[INFO] Python get module succeed." << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	//获取Person类  
	PyObject *pClassPerson = PyObject_GetAttrString(pModule, "Person");//Person:Python文件中的类名  
	// 验证函数是否加载成功
	if (NULL == pClassPerson || !PyCallable_Check(pClassPerson))
	{
		cout << "[ERROR] Can't find funftion (pClassPerson)" << endl;
		return;
	}
	else
	{
		cout << "[INFO] Get function (pClassPerson) succeed." << endl;
	}

	//创建Person类的实例  
	PyObject *pInstancePerson = PyInstance_New(pClassPerson, NULL, NULL);
	if (NULL != pInstancePerson)
	{
		//调用方法  
		PyObject *pReturn = PyObject_CallMethod(pInstancePerson, "greet", "s", "Hello Kitty");   //s表示传递的是字符串,值为"Hello Kitty"
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
	Py_Initialize();    // 初始化

	// 将Python工作路径切换到待调用模块所在目录，一定要保证路径名的正确性
	string path = "../";

	string chdir_cmd = string("sys.path.append(\"") + path + "\")";//结果为"sys.path.append(\"./testxxx.py")"
	const char* cstr_cmd = chdir_cmd.c_str();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString(cstr_cmd);

	// 加载模块
	PyObject* moduleName = PyString_FromString("test001"); //文件名 如：test001.py
	PyObject* pModule = PyImport_Import(moduleName);
	if (!pModule) // 加载模块失败
	{
	 cout << "[ERROR] Python get module failed." << endl;
	 return 0;
	}
	cout << "[INFO] Python get module succeed." << endl;

	// 加载函数
	PyObject* pv = PyObject_GetAttrString(pModule, "add");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
	 cout << "[ERROR] Can't find funftion (add)" << endl;
	 return 0;
	}

	cout << "[INFO] Get function (add) succeed." << endl;

	// 设置参数
	PyObject* args = PyTuple_New(2);   // 2个参数
	PyObject* arg1 = PyInt_FromLong(4);    // 参数一设为4
	PyObject* arg2 = PyInt_FromLong(3);    // 参数二设为3
	PyTuple_SetItem(args, 0, arg1);
	PyTuple_SetItem(args, 1, arg2);

	// 调用函数
	PyObject* pRet = PyObject_CallObject(pv, args);

	// 获取参数
	if (pRet)  // 验证是否调用成功
	{
	 long result = PyInt_AsLong(pRet);
	 cout << "result:" << result;
	}

	Py_Finalize();      //释放资源

	return 0;
}