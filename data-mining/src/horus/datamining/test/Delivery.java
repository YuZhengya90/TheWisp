package horus.datamining.test;

import java.time.DayOfWeek;
import java.time.LocalDate;
import java.util.List;
import horus.datamining.env.Environment;
import horus.datamining.env.EnvironmentImpl;
import horus.datamining.model.*;
import horus.datamining.model.feature.FeatureVector;


public class Delivery
{
	public static void main(String[] args) throws Exception
	{
		Environment environment = new EnvironmentImpl();
		environment.setModelPath("D:/my-git/data-mining/DataMining/models/");
//		deliverGoal1(environment, Goal1Reader.defaultDataFile);
		deliverGoal3(environment, Goal3Reader.defaultDataFile);
	}
	
	
	private static void deliverGoal1(Environment environment, String dataFile) throws Exception
	{
		Model model = new PurchasePricePrediction(environment);
		List<LocalDate> dates = Goal1Reader.readData(dataFile);
		for (LocalDate date : dates)
		{
			FeatureVector featureVector = model.createFeatureVector();
			featureVector.setValue("Year", date.getYear());
			featureVector.setValue("Month", date.getMonthValue());
			featureVector.setValue("Day", date.getDayOfMonth());
			int dayOfWeek = date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);

			Suggestion suggestion = model.solve(featureVector);
			System.out.println(suggestion.getFieldValue("Price"));
		}
	}
	
	
	private static void deliverGoal3(Environment environment, String dataFile) throws Exception
	{
		Model purchasePricePredict = new PurchasePricePrediction(environment);
		Model operationAdvice = new OperationAdvice(environment, true);
		Model saleQuantityPredict = new SaleQuantityPrediction(environment);
		int stock = 0;
		List<Goal3Reader.Item> items = Goal3Reader.readData(dataFile);
		for (Goal3Reader.Item item : items)
		{
			FeatureVector featureVector = null;
			Suggestion suggestion = null;
			int dayOfWeek = 0;
			
			featureVector = purchasePricePredict.createFeatureVector();
			featureVector.setValue("Year", item.date.getYear());
			featureVector.setValue("Month", item.date.getMonthValue());
			featureVector.setValue("Day", item.date.getDayOfMonth());
			dayOfWeek = item.date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);
			suggestion = purchasePricePredict.solve(featureVector);
			System.out.print(suggestion.getFieldValue("Price") + "\t");
			
			featureVector = operationAdvice.createFeatureVector();
			featureVector.setValue("TodayYear", item.date.getYear());
			featureVector.setValue("TodayMonth", item.date.getMonthValue());
			featureVector.setValue("TodayDay", item.date.getDayOfMonth());
			featureVector.setValue("StockQuantity", stock);
			featureVector.setValue("Temperature", item.temperature);
			featureVector.setValue("TargetYear", 2017);
			featureVector.setValue("TargetMonth", 6);
			featureVector.setValue("TargetDay", 30);
			suggestion = operationAdvice.solve(featureVector);
			int purchaseQuantity = ((Number) suggestion.getFieldValue("PurchaseQuantity")).intValue();
			System.out.print(purchaseQuantity + "\t");
			stock += purchaseQuantity;
			
			featureVector = saleQuantityPredict.createFeatureVector();
			featureVector.setValue("Year", item.date.getYear());
			featureVector.setValue("Month", item.date.getMonthValue());
			featureVector.setValue("Day", item.date.getDayOfMonth());
			dayOfWeek = item.date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);
			featureVector.setValue("Comments", item.temperature);
			featureVector.setValue("Price", item.salePrice);
			featureVector.setValue("StockQuantity", stock);
			suggestion = saleQuantityPredict.solve(featureVector);
			int saleQuantity = ((Number) suggestion.getFieldValue("SalesQuantity")).intValue();
			System.out.println(saleQuantity);
			stock -= saleQuantity;
		}
	}
}
