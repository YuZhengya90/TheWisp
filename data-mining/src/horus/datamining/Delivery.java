package horus.datamining;

import java.time.DayOfWeek;
import java.time.LocalDate;
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
		Model operationAdvice = new OperationAdvice(environment, true);
		Model purchasePricePredict = new PurchasePricePrediction(environment);
		Model saleCommentsPredict = new SaleCommentsPrediction(environment);
		Model saleQuantityPredict = new SaleQuantityPrediction(environment);

		LocalDate today = LocalDate.of(2017, 4, 1);
		int stockQuantity = 0;
		LocalDate target = LocalDate.of(2017, 6, 30);
		double profit = 0.0;
		System.out.println("Date\tPurchase Quantity\tSaleQuantity\tStock Quantity\tProfit");
		while (!today.isAfter(target))
		{
			FeatureVector featureVector;
			Suggestion suggestion;
			int dayOfWeek;
			
			featureVector = operationAdvice.createFeatureVector();
			featureVector.setValue("TodayYear", today.getYear());
			featureVector.setValue("TodayMonth", today.getMonthValue());
			featureVector.setValue("TodayDay", today.getDayOfMonth());
			featureVector.setValue("StockQuantity", stockQuantity);
			featureVector.setValue("TargetYear", target.getYear());
			featureVector.setValue("TargetMonth", target.getMonthValue());
			featureVector.setValue("TargetDay", target.getDayOfMonth());
			suggestion = operationAdvice.solve(featureVector);
			double salePrice = ((Number) suggestion.getFieldValue("SalePrice")).doubleValue();
			int purchaseQuantity = ((Number) suggestion.getFieldValue("PurchaseQuantity")).intValue();
			
			featureVector = purchasePricePredict.createFeatureVector();
			featureVector.setValue("Year", today.getYear());
			featureVector.setValue("Month", today.getMonthValue());
			featureVector.setValue("Day", today.getDayOfMonth());
			dayOfWeek = today.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);
			suggestion = purchasePricePredict.solve(featureVector);
			double purchasePrice = ((Number) suggestion.getFieldValue("Price")).doubleValue();
			
			featureVector = saleCommentsPredict.createFeatureVector();
			featureVector.setValue("Year", today.getYear());
			featureVector.setValue("DayOfYear", today.getDayOfYear());
			suggestion = saleCommentsPredict.solve(featureVector);
			double saleComments = ((Number) suggestion.getFieldValue("Comments")).doubleValue();
			
			featureVector = saleQuantityPredict.createFeatureVector();
			featureVector.setValue("Year", today.getYear());
			featureVector.setValue("Month", today.getMonthValue());
			featureVector.setValue("Day", today.getDayOfMonth());
			dayOfWeek = today.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
			featureVector.setValue("WeekDay", dayOfWeek);
			featureVector.setValue("Comments", saleComments);
			featureVector.setValue("Price", salePrice);
			featureVector.setValue("StockQuantity", stockQuantity + purchaseQuantity);
			suggestion = saleQuantityPredict.solve(featureVector);
			int saleQuantity = ((Number) suggestion.getFieldValue("SalesQuantity")).intValue();
			
			stockQuantity += purchaseQuantity - saleQuantity;
			profit += salePrice * saleQuantity - purchasePrice * purchaseQuantity;
			System.out.println(today.toString() + '\t' + purchaseQuantity + '\t' + saleQuantity + '\t' + stockQuantity + '\t' + profit);
			today = today.plusDays(1);
		}
	}
}
