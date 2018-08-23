#include "TPJar.h"

TPJar::TPJar()
	:mInitOK(false), mJVM(nullptr), mJVMEnv(nullptr), mJVMInstance(nullptr)
{	
}

TPJar::~TPJar()
{
	DeInit();
}

void TPJar::PurchasePricePredictionSetModelPath(const char* str)
{
	if (!mInitOK)
	{
		return;
	}

	jclass purchasePricePredictionWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/PurchasePricePredictionWrapper");
	jmethodID setModelPath = mJVMEnv->GetStaticMethodID(purchasePricePredictionWrapper, "setModelPath", "(Ljava/lang/String;)V");
	if (setModelPath == 0)
	{
#ifdef _DEBUG
		cout << "Can't Get SetModelPath" << endl;
#endif
		return;
	}

	mJVMEnv->CallStaticObjectMethod(purchasePricePredictionWrapper, setModelPath, mJVMEnv->NewStringUTF(str));
}

vector<double> TPJar::PurchasePricePredictionPredictPrice(TPDate sDay, TPDate eDay)
{
	vector<double> vecRet;

	if (!mInitOK)
	{
		return vecRet;
	}

	jclass purchasePricePredictionWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/PurchasePricePredictionWrapper");
	jmethodID predictPrice = mJVMEnv->GetStaticMethodID(purchasePricePredictionWrapper, "predictPrice", "(III)[D");
	if (predictPrice == 0)
	{
#ifdef _DEBUG
		cout << "predictPrice Get failed." << endl;
#endif
		return vecRet;
	}

	int dayDistance = eDay - sDay + 1;
	for (int i = 0; i < dayDistance; ++i)
	{
		TPDate oneDay = (sDay + i);
		jdoubleArray dbResult = (jdoubleArray)mJVMEnv->CallStaticObjectMethod(purchasePricePredictionWrapper, 
			predictPrice, oneDay.GetYear(), oneDay.GetMonth(), oneDay.GetDay());
		if (dbResult == 0)
		{
#ifdef _DEBUG
			cout << "predictPrice return 0x00000000" << endl;
#endif
			return vecRet;
		}

		jsize sz = mJVMEnv->GetArrayLength(dbResult);

		jboolean isCopy = true;
		jdouble* dbPtr = mJVMEnv->GetDoubleArrayElements(dbResult, &isCopy);
		vecRet.push_back(dbPtr[0]);

		mJVMEnv->ReleaseDoubleArrayElements(dbResult, dbPtr, 0);
	}

	return vecRet;
}

bool TPJar::Init()
{
	JavaVMInitArgs iniArgs;
	JavaVMOption options[3];
	TPJNI_CreateJVM createJVM = nullptr;

	const int JavaHomePathMax = 256;
	CHAR javaHomePath[JavaHomePathMax] = { 0 };
	GetEnvironmentVariableA("JAVA_HOME", javaHomePath, JavaHomePathMax - 1);

#ifdef _DEBUG
	cout << "JAVA_HOME=" << javaHomePath << endl;
#endif

	memset(&iniArgs, 0, sizeof(JavaVMInitArgs));
	options[0].optionString = "-Djava.compiler=NONE"; // DISABLE JIT
	options[1].optionString = "-Djava.class.path=.;jar/horus.datamining.jar;jar/weka.jar";
	options[2].optionString = "-verbose:NONE";

	iniArgs.version = JNI_VERSION_1_8;
	iniArgs.nOptions = 3;
	iniArgs.options = options;
	iniArgs.ignoreUnrecognized = JNI_TRUE;

	std::string jvmPath = std::string(javaHomePath) + "\\jre\\bin\\client\\jvm.dll";
	
	mJVMInstance = LoadLibraryEx(jvmPath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (mJVMInstance == nullptr)
	{
#ifdef _DEBUG
		cout << "LoadLibraryA:LastError" << GetLastError() << endl;
#endif
		return false;
	}

	createJVM = (TPJNI_CreateJVM)GetProcAddress(mJVMInstance, "JNI_CreateJavaVM");
	if (createJVM(&mJVM, (void**)&mJVMEnv, &iniArgs) < 0)
	{
#ifdef _DEBUG
		cout << "CreateJVM failed!" << endl;
#endif
		return false;
	}

	mInitOK = true;
	return true;
}

void TPJar::DeInit()
{
	if (mJVM != nullptr)
	{
		mJVM->DestroyJavaVM();
	}

	if (mJVMInstance != nullptr)
	{
		FreeLibrary(mJVMInstance);
	}
}