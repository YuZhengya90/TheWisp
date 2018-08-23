#pragma once 

#include "jni.h"
#include <vector>
#include "TPEnvironment.h"
#include "TPConst.h"

using namespace std;

typedef int(__stdcall *TPJNI_CreateJVM)(JavaVM **pvm, void **penv, void *args);

class TPJar
{
public:
	TPJar();
	~TPJar();

	bool Init();
	void PurchasePricePredictionSetModelPath(const char* str);
	vector<double> PurchasePricePredictionPredictPrice(TPDate sDay, TPDate eDay);

private:
	
	void DeInit();

	JavaVM * mJVM;
	JNIEnv * mJVMEnv;
	HINSTANCE mJVMInstance;

	bool mInitOK;
};