package horus.datamining.model;

import java.time.DayOfWeek;
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
import horus.datamining.env.Environment;
import horus.datamining.model.feature.FeatureVector;
import weka.core.Instance;
import weka.core.Instances;

public class OperationAdvice extends AbstractModel
{
	private static int histPurchaseQuantityMax = 966;
	private static int histPurchaseQuantityMin = 141;
	private static double histSalePriceMax = 5.996452;
	private static double histSalePriceMin = 4.980164;
	
	
	private Model purchasePricePrediction;
	private Model salePricePrediction;
	private Model saleQuantityPrediction;
	private Model profitPrediction;
	private boolean isPredictedSalePrice;
	

	public OperationAdvice(Environment environment, boolean isPredictedSalePrice) throws Exception
	{
		super(environment);
		purchasePricePrediction = new PurchasePricePrediction(environment);
		salePricePrediction = new SalePricePrediction(environment);
		saleQuantityPrediction = new SaleQuantityPrediction(environment);
		profitPrediction = new ProfitPrediction(environment);
		this.isPredictedSalePrice = isPredictedSalePrice;
	}


	@Override
	protected String getName()
	{
		return "OperationAdvice";
	}
	

	@Override
	protected Suggestion solveInstances(Instances instances) throws Exception
	{
		Instance instance = instances.get(0);
		int todayYear = (int) instance.value(instances.attribute("TodayYear"));
		int todayMonth = (int) instance.value(instances.attribute("TodayMonth"));
		int todayDay = (int) instance.value(instances.attribute("TodayDay"));
		LocalDate todayDate = LocalDate.of(todayYear, todayMonth, todayDay);
		double stockQuantity = instance.value(instances.attribute("StockQuantity"));
		double saleComments = instance.value(instances.attribute("Temperature"));
		FeatureVector featureVector = null;
		Suggestion suggestion = null;
		
		featureVector = purchasePricePrediction.createFeatureVector();
		featureVector.setValue("Year", todayDate.getYear());
		featureVector.setValue("Month", todayDate.getMonthValue());
		featureVector.setValue("Day", todayDate.getDayOfMonth());
		int dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		featureVector.setValue("WeekDay", dayOfWeek);
		suggestion = purchasePricePrediction.solve(featureVector);
		double purchasePrice = (double) suggestion.getFieldValue("Price");
		
		double salePrice = 0.0;
		if (isPredictedSalePrice)
		{
			featureVector = salePricePrediction.createFeatureVector();
			featureVector.setValue("Year", todayDate.getYear());
			featureVector.setValue("Month", todayDate.getMonthValue());
			featureVector.setValue("Day", todayDate.getDayOfMonth());
			suggestion = salePricePrediction.solve(featureVector);
			salePrice = (double) suggestion.getFieldValue("Price");
		}		
		
		int targetYear = (int) instance.value(instances.attribute("TargetYear"));
		int targetMonth = (int) instance.value(instances.attribute("TargetMonth"));
		int targetDay = (int) instance.value(instances.attribute("TargetDay"));
		LocalDate targetDate = LocalDate.of(targetYear, targetMonth, targetDay);
		
		if (isPredictedSalePrice)
			suggestion = solveWithPredictedSalePrice(todayDate, purchasePrice, stockQuantity, saleComments, salePrice, targetDate);
		else
			suggestion = solveWithoutPredictedSalePrice(todayDate, purchasePrice, stockQuantity, saleComments, targetDate);
		return suggestion;
	}
	
	
	@Override
	protected Suggestion createSuggestion()
	{
		SuggestionImpl suggestion = new SuggestionImpl();
		suggestion.addField("SalePrice");
		suggestion.addField("PurchaseQuantity");
		suggestion.addField("Profit");
		return suggestion;
	}

	
	private Suggestion solveWithPredictedSalePrice(
			LocalDate todayDate,
			double purchasePrice,
			double stockQuantity,
			double saleComments,
			double salePrice,
			LocalDate targetDate) throws Exception
	{
		FeatureVector featureVector = profitPrediction.createFeatureVector();
		featureVector.setValue("TodayYear", todayDate.getYear());
		featureVector.setValue("TodayMonth", todayDate.getMonthValue());
		featureVector.setValue("TodayDay", todayDate.getDayOfMonth());
		int dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		featureVector.setValue("TodayWeekDay", dayOfWeek);
		featureVector.setValue("StockQuantity", stockQuantity);
		featureVector.setValue("PurchasePrice", purchasePrice);
		featureVector.setValue("Comments", saleComments);
		featureVector.setValue("SalePrice", salePrice);
		featureVector.setValue("TargetYear", targetDate.getYear());
		featureVector.setValue("TargetMonth", targetDate.getMonthValue());
		featureVector.setValue("TargetDay", targetDate.getDayOfMonth());
		dayOfWeek = targetDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		featureVector.setValue("TargetWeekDay", dayOfWeek);
		int duration = (int) ChronoUnit.DAYS.between(todayDate, targetDate);
		featureVector.setValue("Duration", duration);
		
		FeatureVector quantityFeatureVector = saleQuantityPrediction.createFeatureVector();
		quantityFeatureVector.setValue("Year", todayDate.getYear());
		quantityFeatureVector.setValue("Month", todayDate.getMonthValue());
		quantityFeatureVector.setValue("Day", todayDate.getDayOfMonth());
		dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		quantityFeatureVector.setValue("WeekDay", dayOfWeek);
		quantityFeatureVector.setValue("Comments", saleComments);
		quantityFeatureVector.setValue("Price", salePrice);

		int purchaseQuantityCeil = (int) (histPurchaseQuantityMax * 1.2);
		int purchaseQuantityFloor = (int) (histPurchaseQuantityMin * 0.8);
		int purchaseQuantityPortion = (purchaseQuantityCeil - purchaseQuantityFloor) / 100;
		purchaseQuantityPortion = (0 == purchaseQuantityPortion ? 1 : purchaseQuantityPortion);
		Suggestion suggestion = null;
		int optimalPurchaseQuantity = 0;
		double optimalProfit = 0.0;
		for (int purchaseQuantity = purchaseQuantityFloor; purchaseQuantity <= purchaseQuantityCeil; purchaseQuantity += purchaseQuantityPortion)
		{
			quantityFeatureVector.setValue("StockQuantity", stockQuantity + purchaseQuantity);
			suggestion = saleQuantityPrediction.solve(quantityFeatureVector);
			int saleQuantity = ((Number) suggestion.getFieldValue("SalesQuantity")).intValue();
			featureVector.setValue("SaleQuantity", saleQuantity);
			featureVector.setValue("PurchaseQuantity", purchaseQuantity);
			
			suggestion = profitPrediction.solve(featureVector);
			double profit = (double) suggestion.getFieldValue("Profit");
			if (profit > optimalProfit)
			{
				optimalProfit = profit;
				optimalPurchaseQuantity = purchaseQuantity;
			}
		}
		
		Suggestion result = createSuggestion();
		result.setFieldValue("SalePrice", salePrice);
		result.setFieldValue("PurchaseQuantity", optimalPurchaseQuantity);
		result.setFieldValue("Profit", optimalProfit);
		return result;
	}
	
	
	private Suggestion solveWithoutPredictedSalePrice(
			LocalDate todayDate,
			double purchasePrice,
			double stockQuantity,
			double saleComments,
			LocalDate targetDate) throws Exception
	{
		FeatureVector featureVector = profitPrediction.createFeatureVector();
		featureVector.setValue("TodayYear", todayDate.getYear());
		featureVector.setValue("TodayMonth", todayDate.getMonthValue());
		featureVector.setValue("TodayDay", todayDate.getDayOfMonth());
		int dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		featureVector.setValue("TodayWeekDay", dayOfWeek);
		featureVector.setValue("StockQuantity", stockQuantity);
		featureVector.setValue("PurchasePrice", purchasePrice);
		featureVector.setValue("Comments", saleComments);
		featureVector.setValue("TargetYear", targetDate.getYear());
		featureVector.setValue("TargetMonth", targetDate.getMonthValue());
		featureVector.setValue("TargetDay", targetDate.getDayOfMonth());
		dayOfWeek = targetDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		featureVector.setValue("TargetWeekDay", dayOfWeek);
		int duration = (int) ChronoUnit.DAYS.between(todayDate, targetDate);
		featureVector.setValue("Duration", duration);
		
		FeatureVector quantityFeatureVector = saleQuantityPrediction.createFeatureVector();
		quantityFeatureVector.setValue("Year", todayDate.getYear());
		quantityFeatureVector.setValue("Month", todayDate.getMonthValue());
		quantityFeatureVector.setValue("Day", todayDate.getDayOfMonth());
		dayOfWeek = todayDate.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		quantityFeatureVector.setValue("WeekDay", dayOfWeek);
		quantityFeatureVector.setValue("Comments", saleComments);

		int purchaseQuantityCeil = (int) (histPurchaseQuantityMax * 1.2);
		int purchaseQuantityFloor = (int) (histPurchaseQuantityMin * 0.8);
		int purchaseQuantityPortion = (purchaseQuantityCeil - purchaseQuantityFloor) / 100;
		purchaseQuantityPortion = (0 == purchaseQuantityPortion ? 1 : purchaseQuantityPortion);
		double salePriceCeil = (histSalePriceMax * 1.2);
		double salePriceFloor = (histSalePriceMin * 0.8);
		double salePricePortion = (salePriceCeil - salePriceFloor) / 100.0;
		Suggestion suggestion = null;
		int optimalPurchaseQuantity = 0;
		double optimalSalePrice = 0.0;
		double optimalProfit = 0.0;
		for (int purchaseQuantity = purchaseQuantityFloor; purchaseQuantity <= purchaseQuantityCeil; purchaseQuantity += purchaseQuantityPortion)
		{
			featureVector.setValue("PurchaseQuantity", purchaseQuantity);
			for (double salePrice = salePriceFloor; salePrice <= salePriceCeil; salePrice += salePricePortion)
			{
				quantityFeatureVector.setValue("Price", salePrice);
				quantityFeatureVector.setValue("StockQuantity", stockQuantity + purchaseQuantity);
				suggestion = saleQuantityPrediction.solve(quantityFeatureVector);
				int saleQuantity = ((Number) suggestion.getFieldValue("SalesQuantity")).intValue();
				featureVector.setValue("SaleQuantity", saleQuantity);
				featureVector.setValue("SalePrice", salePrice);
				
				suggestion = profitPrediction.solve(featureVector);
				double profit = (double) suggestion.getFieldValue("Profit");
				if (profit > optimalProfit)
				{
					optimalProfit = profit;
					optimalPurchaseQuantity = purchaseQuantity;
					optimalSalePrice = salePrice;
				}
			}
		}
		
		Suggestion result = createSuggestion();
		result.setFieldValue("SalePrice", optimalSalePrice);
		result.setFieldValue("PurchaseQuantity", optimalPurchaseQuantity);
		result.setFieldValue("Profit", optimalProfit);
		return result;
	}

}
