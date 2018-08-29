package horus.datamining.wrapper;

import java.time.DayOfWeek;
import java.time.LocalDate;
import horus.datamining.env.Environment;
import horus.datamining.env.EnvironmentImpl;
import horus.datamining.model.*;
import horus.datamining.model.feature.FeatureVector;


public final class SaleQuantityPredictionWrapper
{
	private static Environment environment;
	private static Model commentsModel;
	private static Model quantityModel;


	public static void setModelPath(String modelPath) throws Exception
	{
		environment = new EnvironmentImpl();
		environment.setModelPath(modelPath);
		commentsModel = new SaleCommentsPrediction(environment);
		quantityModel = new SaleQuantityPrediction(environment);
	}


	// Input:
	// 		year - numeric,				(in 2017)
	// 		month - numeric,
	// 		day - numeric,
	// 		price - numeric,			(3.98~7.19)
	// 		stockQuantity - numeric		(0~25000)
	// Output:
	//	 	SalesQuantity - numeric
	public static double[] predictSaleQuantity(int year, int month, int day, double price,
			int stockQuantity)
	{
		double[] result = null;
		try
		{
			LocalDate date = LocalDate.of(year, month, day);

			FeatureVector featureVector = commentsModel.createFeatureVector();
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("DayOfYear", date.getDayOfYear());
			Suggestion suggestion = commentsModel.solve(featureVector);

			featureVector = quantityModel.createFeatureVector();
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("Month", date.getMonthValue());
			featureVector.setValue("Day", date.getDayOfMonth());
			int dayOfWeek = date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);
			featureVector.setValue("Comments", suggestion.getFieldValue("Comments"));
			featureVector.setValue("Price", price);
			featureVector.setValue("StockQuantity", stockQuantity);

			suggestion = quantityModel.solve(featureVector);
			result = new double[1];
			result[0] = ((Number) suggestion.getFieldValue("SalesQuantity")).doubleValue();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}


	public static void test() throws Exception
	{
		SaleQuantityPredictionWrapper.setModelPath("D:/my-git/data-mining/DataMining/models/");
		double[] result = SaleQuantityPredictionWrapper.predictSaleQuantity(2014, 5, 20, 5.99, 21133);
		System.out.println(result[0]);
	}
}
