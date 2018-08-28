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

void TPJar::SalePricePredictionSetModelPath(const char* str)
{
	if (!mInitOK)
	{
		return;
	}

	jclass salePricePredictionWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/SalePricePredictionWrapper");
	jmethodID setModelPath = mJVMEnv->GetStaticMethodID(salePricePredictionWrapper, "setModelPath", "(Ljava/lang/String;)V");
	if (setModelPath == 0)
	{
#ifdef _DEBUG
		cout << "Can't Get SetModelPath" << endl;
#endif
		return;
	}

	mJVMEnv->CallStaticObjectMethod(salePricePredictionWrapper, setModelPath, mJVMEnv->NewStringUTF(str));
}

vector<double> TPJar::SalePricePredictionPredictPrice(TPDate sDay, TPDate eDay)
{
	vector<double> vecRet;

	if (!mInitOK)
	{
		return vecRet;
	}

	jclass salePricePredictionWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/SalePricePredictionWrapper");
	jmethodID predictPrice = mJVMEnv->GetStaticMethodID(salePricePredictionWrapper, "predictPrice", "(III)[D");
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
		jdoubleArray dbResult = (jdoubleArray)mJVMEnv->CallStaticObjectMethod(salePricePredictionWrapper,
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

void TPJar::SaleQuantityPredictionSetModelPath(const char* str)
{
	if (!mInitOK)
	{
		return;
	}

	jclass saleQuantityPredictionWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/SaleQuantityPredictionWrapper");
	jmethodID setModelPath = mJVMEnv->GetStaticMethodID(saleQuantityPredictionWrapper, "setModelPath", "(Ljava/lang/String;)V");
	if (setModelPath == 0)
	{
#ifdef _DEBUG
		cout << "Can't Get SetModelPath" << endl;
#endif
		return;
	}

	mJVMEnv->CallStaticObjectMethod(saleQuantityPredictionWrapper, setModelPath, mJVMEnv->NewStringUTF(str));
}

vector<double> TPJar::SaleQuantityPredictionPredictSaleQuantity(TPDate atDay, float price, int stockQuantity)
{
	vector<double> vecRet;

	if (!mInitOK)
	{
		return vecRet;
	}

	jclass saleQuantityPredictionWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/SaleQuantityPredictionWrapper");
	jmethodID predictSaleQuantity = mJVMEnv->GetStaticMethodID(saleQuantityPredictionWrapper, "predictSaleQuantity", "(IIIDI)[D");
	if (predictSaleQuantity == 0)
	{
#ifdef _DEBUG
		cout << "predictSaleQuantity Get failed." << endl;
#endif
		return vecRet;
	}

	jdoubleArray dbResult = (jdoubleArray)mJVMEnv->CallStaticObjectMethod(saleQuantityPredictionWrapper,
		predictSaleQuantity, atDay.GetYear(), atDay.GetMonth(), atDay.GetDay(), (jdouble)price, stockQuantity);
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

	return vecRet;
}

void TPJar::ProfitPredictionSetModelPath(const char* str)
{
	if (!mInitOK)
	{
		return;
	}

	jclass profitPredictionWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/ProfitPredictionWrapper");
	jmethodID setModelPath = mJVMEnv->GetStaticMethodID(profitPredictionWrapper, "setModelPath", "(Ljava/lang/String;)V");
	if (setModelPath == 0)
	{
#ifdef _DEBUG
		cout << "Can't Get SetModelPath" << endl;
#endif
		return;
	}

	mJVMEnv->CallStaticObjectMethod(profitPredictionWrapper, setModelPath, mJVMEnv->NewStringUTF(str));
}

vector<double> TPJar::ProfitPredictionPredictProfit(
	TPDate today, TPDate targetDay, int stockQuantity, float purchasePrice, int purchaseQuantity, float salePrice)
{
	vector<double> vecRet;

	if (!mInitOK)
	{
		return vecRet;
	}

	jclass profitPredictionWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/ProfitPredictionWrapper");
	jmethodID predictProfit = mJVMEnv->GetStaticMethodID(profitPredictionWrapper, "predictProfit", "(IIIIDIDIII)[D");
	if (predictProfit == 0)
	{
#ifdef _DEBUG
		cout << "predictProfit Get failed." << endl;
#endif
		return vecRet;
	}

	int distance = targetDay - today + 1;
	for (int i = 0; i < distance; ++i)
	{
		TPDate nextDay = today + i;
		jdoubleArray dbResult = (jdoubleArray)mJVMEnv->CallStaticObjectMethod(profitPredictionWrapper,
			predictProfit, today.GetYear(), today.GetMonth(), today.GetDay(), stockQuantity, purchasePrice, purchaseQuantity, salePrice
			, nextDay.GetYear(), nextDay.GetMonth(), nextDay.GetDay());
		if (dbResult == 0)
		{
#ifdef _DEBUG
			cout << "predictProfit return 0x00000000" << endl;
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

void TPJar::OperationAdviceSetModelPath(const char* str)
{
	if (!mInitOK)
	{
		return;
	}

	jclass operationAdviceWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/OperationAdviceWrapper");
	jmethodID setModelPath = mJVMEnv->GetStaticMethodID(operationAdviceWrapper, "setModelPath", "(Ljava/lang/String;)V");
	if (setModelPath == 0)
	{
#ifdef _DEBUG
		cout << "Can't Get SetModelPath" << endl;
#endif
		return;
	}

	mJVMEnv->CallStaticObjectMethod(operationAdviceWrapper, setModelPath, mJVMEnv->NewStringUTF(str));
}

vector<double> TPJar::OperationAdviceAdvice(TPDate today, TPDate targetDay, int stockQuantity)
{
	vector<double> vecRet;

	if (!mInitOK)
	{
		return vecRet;
	}

	jclass operationAdviceWrapper = mJVMEnv->FindClass("horus/datamining/wrapper/OperationAdviceWrapper");
	jmethodID advice = mJVMEnv->GetStaticMethodID(operationAdviceWrapper, "advice", "(IIIIIII)[D");
	if (advice == 0)
	{
#ifdef _DEBUG
		cout << "advice Get failed." << endl;
#endif
		return vecRet;
	}

	jdoubleArray dbResult = (jdoubleArray)mJVMEnv->CallStaticObjectMethod(operationAdviceWrapper, advice
		, today.GetYear(), today.GetMonth(), today.GetDay(), stockQuantity
		, targetDay.GetYear(), targetDay.GetMonth(), targetDay.GetDay());
	if (dbResult == 0)
	{
#ifdef _DEBUG
		cout << "OperationAdviceAdvice return 0x00000000" << endl;
#endif
		return vecRet;
	}

	jsize sz = mJVMEnv->GetArrayLength(dbResult);
	if (sz != 3)
	{
#ifdef _DEBUG
		cout << "size of Advice result is error!" << endl;
#endif
	}

	jboolean isCopy = true;
	jdouble* dbPtr = mJVMEnv->GetDoubleArrayElements(dbResult, &isCopy);
	vecRet.push_back(dbPtr[0]);
	vecRet.push_back(dbPtr[1]);
	vecRet.push_back(dbPtr[2]);

	mJVMEnv->ReleaseDoubleArrayElements(dbResult, dbPtr, 0);

	return vecRet;
}

bool TPJar::Init(const char* modelPath)
{
	JavaVMInitArgs iniArgs;
	JavaVMOption options[3];
	TPJNI_CreateJVM createJVM = nullptr;

	const int JavaHomePathMax = 256;
	CHAR javaHomePath[JavaHomePathMax] = { 0 };
	GetEnvironmentVariableA("JAVA_HOME", javaHomePath, JavaHomePathMax - 1);
    strcpy_s(javaHomePath, "C:\\Program Files (x86)\\Java\\jdk1.8.0_181");

#ifdef _DEBUG
	cout << "JAVA_HOME=" << javaHomePath << endl;
#endif

	memset(&iniArgs, 0, sizeof(JavaVMInitArgs));
	options[0].optionString = "-Djava.compiler=NONE"; // DISABLE JIT
	options[1].optionString = "-Djava.class.path=.;data-mining/lib/horus.datamining.jar;data-mining/lib/weka.jar";
	options[2].optionString = "-verbose:NONE";

	iniArgs.version = JNI_VERSION_1_8;
	iniArgs.nOptions = 3;
	iniArgs.options = options;
	iniArgs.ignoreUnrecognized = JNI_TRUE;


	std::string jvmPath = std::string(javaHomePath) + "\\jre\\bin\\server\\jvm.dll";
	
	mJVMInstance = LoadLibraryA(jvmPath.c_str());
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
	PurchasePricePredictionSetModelPath(modelPath);
	SalePricePredictionSetModelPath(modelPath);
	SaleQuantityPredictionSetModelPath(modelPath);
	ProfitPredictionSetModelPath(modelPath);
	OperationAdviceSetModelPath(modelPath);
	
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