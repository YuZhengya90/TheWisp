package horus.datamining.wrapper;

import java.time.DayOfWeek;
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
import horus.datamining.env.Environment;
import horus.datamining.env.EnvironmentImpl;
import horus.datamining.model.*;
import horus.datamining.model.feature.FeatureVector;

public class ProfitPredictionWrapper
{
	private static Environment environment;
	private static Model commentsModel;
	private static Model saleQuantityModel;
	private static Model profitModel;


	public static void setModelPath(String modelPath) throws Exception
	{
		environment = new EnvironmentImpl();
		environment.setModelPath(modelPath);
		commentsModel = new SaleCommentsPrediction(environment);
		saleQuantityModel = new SaleQuantityPrediction(environment);
		profitModel = new ProfitPrediction(environment);
	}
	
	
	// Input:
	//		todayYear - numeric
	//		todayMonth - numeric
	//		todayDay - numeric
	//		stockQuantity - numeric
	//		purchasePrice - numeric
	//		purchaseQuantity - numeric
	//		salePrice - numeric
	//		targetYear - numeric
	//		targetMonth - numeric
	//		targetDay - numeric
	//	Output:
	//		Profit - numeric
	public static double[] predictProfit(
			int todayYear,
			int todayMonth,
			int todayDay,
			int stockQuantity,
			double purchasePrice,
			int purchaseQuantity,
			double salePrice,
			int targetYear,
			int targetMonth,
			int targetDay)
	{
		double[] result = null;
		try
		{
			LocalDate todayDate = LocalDate.of(todayYear, todayMonth, todayDay);
			FeatureVector featureVector = null;
			Suggestion suggestion = null;
			int dayOfWeek = 0;
			
			featureVector = commentsModel.createFeatureVector();
			featureVector.setValue("Year", todayDate.getYear());
			featureVector.setValue("DayOfYear", todayDate.getDayOfYear());
			suggestion = commentsModel.solve(featureVector);
			double comments = (double) suggestion.getFieldValue("Comments");
			
			featureVector = saleQuantityModel.createFeatureVector();
			featureVector.setValue("Year", todayDate.getYear());
			featureVector.setValue("Month", todayDate.getMonthValue());
			featureVector.setValue("Day", todayDate.getDayOfMonth());
			dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);
			featureVector.setValue("Comments", comments);
			featureVector.setValue("Price", salePrice);
			featureVector.setValue("StockQuantity", stockQuantity);
			suggestion = saleQuantityModel.solve(featureVector);
			int saleQuantity = ((Number) suggestion.getFieldValue("SalesQuantity")).intValue();
			
			LocalDate targetDate = LocalDate.of(targetYear, targetMonth, targetDay);
			featureVector = profitModel.createFeatureVector();
			featureVector.setValue("TodayYear", todayDate.getYear());
			featureVector.setValue("TodayMonth", todayDate.getMonthValue());
			featureVector.setValue("TodayDay", todayDate.getDayOfMonth());
			dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("TodayWeekDay", dayOfWeek);
			featureVector.setValue("StockQuantity", stockQuantity);
			featureVector.setValue("PurchasePrice", purchasePrice);
			featureVector.setValue("PurchaseQuantity", purchaseQuantity);
			featureVector.setValue("Comments", comments);
			featureVector.setValue("SalePrice", salePrice);
			featureVector.setValue("SaleQuantity", saleQuantity);
			featureVector.setValue("TargetYear", targetDate.getYear());
			featureVector.setValue("TargetMonth", targetDate.getMonthValue());
			featureVector.setValue("TargetDay", targetDate.getDayOfMonth());
			dayOfWeek = targetDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("TargetWeekDay", dayOfWeek);
			int duration = (int) ChronoUnit.DAYS.between(todayDate, targetDate);
			featureVector.setValue("Duration", duration);
			
			suggestion = profitModel.solve(featureVector);
			result = new double[1];
			result[0] = (double) suggestion.getFieldValue("Profit");
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	
	public static void test() throws Exception
	{
		ProfitPredictionWrapper.setModelPath("D:/my-git/data-mining/DataMining/models/");
		double[] result = ProfitPredictionWrapper.predictProfit(
				2017,4,1,0,2.334107431547618,235,4.99,2017,6,30);
		System.out.println(result[0]);
	}
}
