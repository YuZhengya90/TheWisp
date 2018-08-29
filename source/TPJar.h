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

	bool Init(const char* modelPath);

	void PurchasePricePredictionSetModelPath(const char* str);
	void SalePricePredictionSetModelPath(const char* str);
	void SaleQuantityPredictionSetModelPath(const char* str);
	void ProfitPredictionSetModelPath(const char* str);
	void OperationAdviceSetModelPath(const char* str);

	// return [sDay, eDay]'s purchase price vector.
	vector<double> PurchasePricePredictionPredictPrice(TPDate sDay, TPDate eDay);

	// return [sDay, eDay]'s sales price vector.
	vector<double> SalePricePredictionPredictPrice(TPDate sDay, TPDate eDay);

	// return the [AtDay]'s sale quantity.
	vector<double> SaleQuantityPredictionPredictSaleQuantity(TPDate atDay, float price, int stockQuantity);

	// return [today, targetDay]'s profit vector.
	vector<double> ProfitPredictionPredictProfit(
		TPDate today, TPDate targetDay, int stockQuantity, float purchasePrice, int purchaseQuantity, float salePrice);

	// return the [SalePrice,PurchaseQuantity,Profit]
	vector<double> OperationAdviceAdvice(TPDate today, TPDate targetDay, int stockQuantity);

private:

	void DeInit();

	JavaVM * mJVM;
	JNIEnv * mJVMEnv;
	HINSTANCE mJVMInstance;

	bool mInitOK;
};