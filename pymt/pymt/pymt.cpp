// pymt.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "Python.h"
#include <string>

using namespace std;

extern "C" __declspec(dllexport) void LoadScript(wchar_t* name);
extern "C" __declspec(dllexport) void Finalize();
extern "C" __declspec(dllexport) void NewArg(int i);
extern "C" __declspec(dllexport) void CallFunc(wchar_t* name);
extern "C" __declspec(dllexport) long GetRezLong();

PyObject *pModule;
PyObject *pFunc;
PyObject *pArgs;
PyObject *pValue;

void LoadScript(wchar_t* name)
{
	wstring w(name);
	string st(w.begin(), w.end());
	const char * s = st.c_str();
	Py_Initialize();
	FILE* fp;
	fopen_s(&fp, s, "r");
	PyRun_SimpleFile(fp, s);
	pModule = PyImport_AddModule("__ main__");
}

void Finalize()
{
	Py_DECREF(pModule);
	Py_Finalize();
}

void NewArg(int i)
{
	pArgs = PyTuple_New(i);
}

void CallFunc(wchar_t* name)
{
	wstring w(name);
	string st(w.begin(), w.end());
	const char * s = st.c_str();
	pFunc = PyObject_GetAttrString(pModule, s);
	if (pFunc && PyCallable_Check(pFunc))
	{
		pValue = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);
	}
}

long GetRezLong()
{
	long rez = 0;
	if (pValue != NULL)
	{
		rez = PyLong_AsLong(pValue);
		Py_DECREF(pValue);
	}
	return rez;
}
