package horus.datamining.test;

import java.time.DayOfWeek;
import java.time.LocalDate;
import horus.datamining.env.Environment;
import horus.datamining.env.EnvironmentImpl;
import horus.datamining.model.*;
import horus.datamining.model.feature.FeatureVector;
import horus.datamining.wrapper.PurchasePricePredictionWrapper;


public final class Sample
{
	public static void main(String[] args) throws Exception
	{
		directCallSample();
		wrapperSample();
	}
	
	
	public static void directCallSample() throws Exception
	{
		System.out.println("Sample of direct call******************");
		// Set environment: set path that contains .model and .arff files
		Environment environment = new EnvironmentImpl();
		environment.setModelPath("D:/my-git/data-mining/DataMining/models/");
		
		// Create model accordingly
		Model model = new PurchasePricePrediction(environment);
		
		// Create feature vector
		// Schema could be found in corresponding .arff files
		// Typically, the last feature is the result feature
		FeatureVector featureVector = model.createFeatureVector();
		LocalDate date = LocalDate.of(2017, 4, 1);
		featureVector.setValue("Year", date.getYear());
		featureVector.setValue("Month", date.getMonthValue());
		featureVector.setValue("Day", date.getDayOfMonth());
		int dayOfWeek = date.getDayOfWeek().getValue() % DayOfWeek.SUNDAY.getValue();
		featureVector.setValue("WeekDay", dayOfWeek);
		System.out.println("Feature Vector" +
				": Year - " + date.getYear() +
				", Month - " + date.getMonthValue() +
				", Day - " + date.getDayOfMonth() +
				", WeekDay - " + dayOfWeek);
		
		
		// Solve the feature vector by the model
		// Suggestion contains several fields
		// Typically the last feature of the feature schema
		Suggestion suggestion = model.solve(featureVector);
		System.out.println("Predicted price: " + suggestion.getFieldValue("Price"));
	}
	
	
	public static void wrapperSample() throws Exception
	{
		// Model path need to be specified before prediction
		System.out.println("Sample of wrapper**********************");
		PurchasePricePredictionWrapper.setModelPath("D:/my-git/data-mining/DataMining/models/");
		
		// Prediction will be based on the .model files in the specified model path
		// 1st prediction
		System.out.println("Predicting price of 2017-04-01...");
		double[] result = PurchasePricePredictionWrapper.predictPrice(2017, 4, 1);
		System.out.println("Predicted price: " + result[0]);
		// 2nd prediction
		System.out.println("Predicting price of 2017-04-02...");
		result = PurchasePricePredictionWrapper.predictPrice(2017, 4, 2);
		System.out.println("Predicted price: " + result[0]);
	}
}
