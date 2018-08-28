package horus.datamining.wrapper;

import horus.datamining.env.Environment;
import horus.datamining.env.EnvironmentImpl;
import horus.datamining.model.OperationAdvice;
import horus.datamining.model.Suggestion;
import horus.datamining.model.feature.FeatureVector;


public class OperationAdviceWrapper
{
	private static Environment environment;
	private static OperationAdvice model;


	public static void setModelPath(String modelPath) throws Exception
	{
		environment = new EnvironmentImpl();
		environment.setModelPath(modelPath);
		model = new OperationAdvice(environment, false);
	}


	//	Input:
	//		todayYear - numeric
	//		todayMonth - numeric
	//		todayDay - numeric
	//		stockQuantity - numeric
	//		targetYear - numeric
	//		targetMonth - numeric
	//		targetDay - numeric
	//	Output:
	//		SalePrice - numeric
	//		PurchaseQuantity - numeric
	//		Profit - numeric
	public static double[] advice(int todayYear, int todayMonth, int todayDay, int stockQuantity, int targetYear,
			int targetMonth, int targetDay)
	{
		double[] result = null;
		try
		{
			FeatureVector featureVector = model.createFeatureVector();
			featureVector.setValue("TodayYear", todayYear);
			featureVector.setValue("TodayMonth", todayMonth);
			featureVector.setValue("TodayDay", todayDay);
			featureVector.setValue("StockQuantity", stockQuantity);
			featureVector.setValue("TargetYear", targetYear);
			featureVector.setValue("TargetMonth", targetMonth);
			featureVector.setValue("TargetDay", targetDay);

			Suggestion suggestion = model.solve(featureVector);
			result = new double[3];
			result[0] = ((Number) suggestion.getFieldValue("SalePrice")).doubleValue();
			result[1] = ((Number) suggestion.getFieldValue("PurchaseQuantity")).doubleValue();
			result[2] = ((Number) suggestion.getFieldValue("Profit")).doubleValue();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}


	public static void test() throws Exception
	{
		OperationAdviceWrapper.setModelPath("D:/my-git/data-mining/DataMining/models/");
		double[] result = OperationAdviceWrapper.advice(2017, 4, 1, 0, 2017, 6, 30);
		System.out.println("SalePrice - " + result[0]);
		System.out.println("PurchaseQuantity - " + result[1]);
		System.out.println("Profit - " + result[2]);
	}
}
