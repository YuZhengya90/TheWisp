package horus.datamining.test;

import java.time.DayOfWeek;
import java.time.LocalDate;
import horus.datamining.env.Environment;
import horus.datamining.env.EnvironmentImpl;
import horus.datamining.model.*;
import horus.datamining.model.feature.FeatureVector;
import horus.datamining.wrapper.OperationAdviceWrapper;
import horus.datamining.wrapper.ProfitPredictionWrapper;
import horus.datamining.wrapper.SaleQuantityPredictionWrapper;
import horus.datamining.wrapper.TemperaturePredictionWrapper;


public final class Test
{
	public static void main(String[] args) throws Exception
	{
		testOperationAdvice();
		OperationAdviceWrapper.test();
	}


	private static void testPurchasePricePrediction() throws Exception
	{
		Environment environment = new EnvironmentImpl();
		environment.setModelPath("D:/my-git/data-mining/DataMining/models/");
		Model model = new PurchasePricePrediction(environment);

		LocalDate date = LocalDate.of(2017, 4, 1);
		LocalDate endDate = LocalDate.of(2017, 12, 31);
		while (!date.isAfter(endDate))
		{
			FeatureVector featureVector = model.createFeatureVector();
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("Month", date.getMonthValue());
			featureVector.setValue("Day", date.getDayOfMonth());
			int dayOfWeek = date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);

			Suggestion suggestion = model.solve(featureVector);
			System.out.println(suggestion.getFieldValue("Price"));
			date = date.plusDays(1);
		}
	}


	private static void testSaleCommentPrediction() throws Exception
	{
		Environment environment = new EnvironmentImpl();
		environment.setModelPath("D:/my-git/data-mining/DataMining/models/");
		Model model = new SaleCommentsPrediction(environment);

		LocalDate date = LocalDate.of(2017, 4, 1);
		LocalDate endDate = LocalDate.of(2017, 12, 31);
		while (!date.isAfter(endDate))
		{
			FeatureVector featureVector = model.createFeatureVector();
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("DayOfYear", date.getDayOfYear());

			Suggestion suggestion = model.solve(featureVector);
			System.out.println(suggestion.getFieldValue("Comments"));
			date = date.plusDays(1);
		}
	}


	private static void testSaleQuantityPrediction() throws Exception
	{
		Environment environment = new EnvironmentImpl();
		environment.setModelPath("D:/my-git/data-mining/DataMining/models/");

		Model commentsModel = new SaleCommentsPrediction(environment);
		Model quantityModel = new SaleQuantityPrediction(environment);
		Model salePriceModel = new SalePricePrediction(environment);

		LocalDate date = LocalDate.of(2017, 4, 1);
		LocalDate endDate = LocalDate.of(2017, 12, 31);
		while (!date.isAfter(endDate))
		{
			FeatureVector featureVector = commentsModel.createFeatureVector();
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("DayOfYear", date.getDayOfYear());
			Suggestion suggestion = commentsModel.solve(featureVector);
			
			
			featureVector = salePriceModel.createFeatureVector();
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("Month", date.getMonthValue());
			featureVector.setValue("Day", date.getDayOfMonth());
			suggestion = salePriceModel.solve(featureVector);
			double salePrice = (double) suggestion.getFieldValue("Price");

			featureVector = quantityModel.createFeatureVector();
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("Month", date.getMonthValue());
			featureVector.setValue("Day", date.getDayOfMonth());
			int dayOfWeek = date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);
			featureVector.setValue("Comments", suggestion.getFieldValue("Comments"));
			featureVector.setValue("Price", salePrice);
			featureVector.setValue("StockQuantity", 3621); // average of 2017

			suggestion = quantityModel.solve(featureVector);
			System.out.println(suggestion.getFieldValue("SalesQuantity"));
			date = date.plusDays(1);
		}
	}


	private static void testProfitPrediction() throws Exception
	{
		Environment environment = new EnvironmentImpl();
		environment.setModelPath("D:/my-git/data-mining/DataMining/models/");
		FeatureVector featureVector = null;
		Suggestion suggestion = null;
		int dayOfWeek = 0;
		LocalDate todayDate = LocalDate.of(2017, 4, 1);

		Model purchasePriceModel = new PurchasePricePrediction(environment);
		featureVector = purchasePriceModel.createFeatureVector();
		featureVector.setValue("Year", todayDate.getYear());
		featureVector.setValue("Month", todayDate.getMonthValue());
		featureVector.setValue("Day", todayDate.getDayOfMonth());
		dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		featureVector.setValue("WeekDay", dayOfWeek);
		suggestion = purchasePriceModel.solve(featureVector);
		double purchasePrice = (double) suggestion.getFieldValue("Price");

		Model commentsModel = new SaleCommentsPrediction(environment);
		featureVector = commentsModel.createFeatureVector();
		featureVector.setValue("Year", todayDate.getYear());
		featureVector.setValue("DayOfYear", todayDate.getDayOfYear());
		suggestion = commentsModel.solve(featureVector);
		double comments = (double) suggestion.getFieldValue("Comments");
		
		Model salePriceModel = new SalePricePrediction(environment);
		featureVector = salePriceModel.createFeatureVector();
		featureVector.setValue("Year", todayDate.getYear());
		featureVector.setValue("Month", todayDate.getMonthValue());
		featureVector.setValue("Day", todayDate.getDayOfMonth());
		suggestion = salePriceModel.solve(featureVector);
		double salePrice = (double) suggestion.getFieldValue("Price");

		Model saleQuantityModel = new SaleQuantityPrediction(environment);
		featureVector = saleQuantityModel.createFeatureVector();
		featureVector.setValue("Year", todayDate.getYear());
		featureVector.setValue("Month", todayDate.getMonthValue());
		featureVector.setValue("Day", todayDate.getDayOfMonth());
		dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		featureVector.setValue("WeekDay", dayOfWeek);
		featureVector.setValue("Comments", comments);
		featureVector.setValue("Price", salePrice);
		featureVector.setValue("StockQuantity", 0);
		suggestion = saleQuantityModel.solve(featureVector);
		int saleQuantity = ((Number) suggestion.getFieldValue("SalesQuantity")).intValue();

		Model profitModel = new ProfitPrediction(environment);
		LocalDate targetDate = todayDate;
		LocalDate endDate = LocalDate.of(2017, 6, 30);
		int duration = 0;
		while (!targetDate.isAfter(endDate))
		{
			featureVector = profitModel.createFeatureVector();
			featureVector.setValue("TodayYear", todayDate.getYear());
			featureVector.setValue("TodayMonth", todayDate.getMonthValue());
			featureVector.setValue("TodayDay", todayDate.getDayOfMonth());
			dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("TodayWeekDay", dayOfWeek);
			featureVector.setValue("StockQuantity", 0);
			featureVector.setValue("PurchasePrice", purchasePrice);
			featureVector.setValue("PurchaseQuantity", 235);	// mean from 20170101 to 20170331
			featureVector.setValue("Comments", comments);
			featureVector.setValue("SalePrice", salePrice);
			featureVector.setValue("SaleQuantity", saleQuantity);
			featureVector.setValue("TargetYear", targetDate.getYear());
			featureVector.setValue("TargetMonth", targetDate.getMonthValue());
			featureVector.setValue("TargetDay", targetDate.getDayOfMonth());
			dayOfWeek = targetDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("TargetWeekDay", dayOfWeek);
			featureVector.setValue("Duration", duration);

			suggestion = profitModel.solve(featureVector);
			System.out.println(suggestion.getFieldValue("Profit"));
			targetDate = targetDate.plusDays(1);
			++duration;
		}
	}


	private static void testSalePricePrediction() throws Exception
	{
		Environment environment = new EnvironmentImpl();
		environment.setModelPath("D:/my-git/data-mining/DataMining/models/");
		Model model = new SalePricePrediction(environment);

		LocalDate date = LocalDate.of(2017, 4, 1);
		LocalDate endDate = LocalDate.of(2017, 12, 31);
		while (!date.isAfter(endDate))
		{
			FeatureVector featureVector = model.createFeatureVector();
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("Month", date.getMonthValue());
			featureVector.setValue("Day", date.getDayOfMonth());

			Suggestion suggestion = model.solve(featureVector);
			System.out.println(suggestion.getFieldValue("Price"));
			date = date.plusDays(1);
		}
	}
	
	
	private static void testOperationAdvice() throws Exception
	{
		Environment environment = new EnvironmentImpl();
		environment.setModelPath("D:/my-git/data-mining/DataMining/models/");
		Model model = new OperationAdvice(environment, true);
		
		FeatureVector featureVector = model.createFeatureVector();
		featureVector.setValue("TodayYear", 2017);
		featureVector.setValue("TodayMonth", 4);
		featureVector.setValue("TodayDay", 1);
		featureVector.setValue("StockQuantity", 0);
		featureVector.setValue("Temperature", 14.57);
		featureVector.setValue("TargetYear", 2017);
		featureVector.setValue("TargetMonth", 6);
		featureVector.setValue("TargetDay", 30);
		Suggestion suggestion = model.solve(featureVector);
		System.out.println("SalePrice - " + suggestion.getFieldValue("SalePrice"));
		System.out.println("PurchaseQuantity - " + suggestion.getFieldValue("PurchaseQuantity"));
		System.out.println("Profit - " + suggestion.getFieldValue("Profit"));
	}
}
